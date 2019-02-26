using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PCverPIcking : MonoBehaviour
{
    public Camera mainCamera;
    public GameObject myPlanet;
    private GameObject PickedMeshObj;
    public static bool isDominatedCheck; // 턴이 넘어가지 않아 아직 미정인 상태
    public static bool isDominatedConfirm; // 턴이 넘어가서 확정된 상태
    RaycastHit hit;
    public CameraShake cameraShake;
    public float duration;
    public float magnitude;
    private bool bDomination;

    private bool bSelectionFlag;
    private bool bDominateMyFlag;
    private bool bDominateEnemyFlag;
    public GameObject enemyFlag;
    public GameObject myFlag;

    private GameObject networkSystem;
    private FlowSystem flowSystem;

    private void Start()
    {
        myPlanet = GameObject.FindWithTag("InGamePlanet");
        isDominatedCheck = false;
        isDominatedConfirm = false;

        networkSystem = GameObject.Find("NetworkManager");
        flowSystem = GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>();
    }

    private void Update()
    {
        if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGCARD))
        {
            myPlanet.GetComponent<AllMeshController>().PickContainer.Clear();
            CameraController.ChangeableCount = CameraController.DiceCount;

            //Debug.Log("ChangeableCount : " + CameraController.ChangeableCount);

        } // 카드 피킹할 때 주사위 값 받아오기

        if (TurnSystem.isSetTerrainDone)
        {
            //Debug.Log("Enemy Turn");
            if (CameraController.Once == true) // 내 턴에서 넘어갈 때 한번만
            {
                TurnChangeLogic();
                Debug.Log("한번만 ?");
            }
        }

        if (Input.GetMouseButton(0) && CameraController.offset < 0.5)
        {
            Picked(false);
        }
    }

    public void Picked(bool isMoblie)
    {
        Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
        RaycastHit hitObj;

        //19.01.15 YSH 추가합니다..
        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGEVENTCARDLOC))
        {
            if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
            {
                PickedMeshObj = hitObj.transform.gameObject;

                switch(EventCardManager.selectedIndex)
                {
                    case 101:
                        if (!PickedMeshObj.GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))   //To enemy
                            return;
                        break;
                    case 111:
                        if (!PickedMeshObj.GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))   //To enemy
                            return;
                        break;
                    case 201:
                        if (!PickedMeshObj.GetComponent<MeshController>().currentIdentify.Equals(Identify.ALLY))
                            return;
                        break;
                    case 202:
                        if (!PickedMeshObj.GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))   //To enemy
                            return;
                        break;
                }
                
                AllMeshController.IngameManager.GetComponent<EventCardManager>().PickLocDone(PickedMeshObj, PickedMeshObj.GetComponent<MeshController>().terrainstate);

            }
        }

        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow != FLOW.TO_PICKINGLOC)
            return;

        if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
        {
            PickedMeshObj = hitObj.transform.gameObject;
            
            //if (PickedMeshObj.GetComponent<MeshController>() == null)
            //{
            //    PickedMeshObj.AddComponent<MeshController>();
            //}

            if (!PickedMeshObj.GetComponent<MeshController>().isFixed && !PickedMeshObj.GetComponent<MeshController>().isAwake)
            {
                if (myPlanet.GetComponent<AllMeshController>().isEmpty()) // 턴 시작후 첫번째 로직
                {
                    //점령이 끝난 뒤 첫턴 로직 (다른 곳도 선택할 수 있도록)
                    if (isDominatedConfirm == true)
                    {
                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                        myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                    }
                    else // 점령전 (내 Flag와 내 flagable만 선택할 수 있어야 함.)
                    {
                        if (myPlanet.GetComponent<AllMeshController>().myFlag == null && PickedMeshObj.GetComponent<MeshController>().isFlag == true) // Flag하나만 선택할 수 있어야 해.
                        {
                            PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                            myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                            myPlanet.GetComponent<AllMeshController>().myFlag = PickedMeshObj;
                            GameObject.FindWithTag("GameManager").GetComponent<InGameSceneManager>().diceValueForLoop++;
                        }
                        else // 깃발 획득했지만, 아직 점령 전일 때 (내 NearMesh를 선택할 수 있어야 해.)
                        {
                            if (myPlanet.GetComponent<AllMeshController>().myFlag != null) {
                                for (int i = 0; i < myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; i++)
                                {
                                    if (myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[i] == PickedMeshObj)
                                    {
                                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                        myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                                    }
                                }
                            }
                        }
                    }
                }
                else // 턴 시작 후 두번째 이후 로직
                {
                    if (PickedMeshObj.GetComponent<MeshController>().terrainstate == Terrain.DEFAULT || // DEFALT는 더이상 들어가면 안됨.
                        PickedMeshObj.GetComponent<MeshController>().terrainstate == Terrain.FLAG) // FLAG는 이제 못지움
                        return;

                    // 제거 부분
                    if (myPlanet.GetComponent<AllMeshController>().PickContainer.Contains(PickedMeshObj.GetComponent<MeshController>().MeshNumber))
                    {
                        // 다시 누른 거
                        if (myPlanet.GetComponent<AllMeshController>().PickContainer[0] == PickedMeshObj.GetComponent<MeshController>().MeshNumber ||
                            myPlanet.GetComponent<AllMeshController>().PickContainer[myPlanet.GetComponent<AllMeshController>().PickContainer.Count - 1]
                            == PickedMeshObj.GetComponent<MeshController>().MeshNumber) // 첫 값 혹은 마지막 값이야
                        {
                            if (CameraController.ChangeableCount < CameraController.DiceCount + 1)
                            {
                                myPlanet.GetComponent<AllMeshController>().PickContainer.Remove(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                            }
                        }
                    }
                    else // 추가 부분
                    {
                        if (CameraController.ChangeableCount > 0)
                        {
                            // 처음 누른거
                            bool temp = false;
                            // 0번의 Joint에 있는 애면 0번으로 들어가!
                            for (int i = 0; i < 3; i++)
                            {
                                if (GameObject.Find(myPlanet.GetComponent<AllMeshController>().PickContainer[0].ToString()).GetComponent<MeshController>().JointMesh[i].name
                                    == PickedMeshObj.GetComponent<MeshController>().MeshNumber.ToString())
                                {
                                    //Debug.Log("앞 쪽 꼬리에 들어갑니다.");
                                    myPlanet.GetComponent<AllMeshController>().PickContainer.Insert(0, PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                                    PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                    temp = true;
                                    break;
                                }
                            }
                            if (temp == false) // 0번에 있는 애가 아니면 그냥 뒤에 붙어
                            {
                                myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber); // 값을 추가
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                            }
                        }
                    }
                }
            }

            CameraController.offset = 2.0f;
        }
        CameraController.Once = true;
    }

    public void TurnChangeLogic()
    {
        int Length = myPlanet.GetComponent<AllMeshController>().PickContainer.Count;
        AllMeshController.giveLinkNum++;
        //Debug.Log("내 턴에서 넘어갈 때 한번만");
        for (int i = 0; i < Length; i++) // 링크드넘버 세팅
        {
            GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());
            FindObject.GetComponent<MeshController>().LinkedNumber = AllMeshController.giveLinkNum;
        }

        for (int i = 0; i < Length; i++)
        {
            GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());

            if (isDominatedCheck == true)
            {
                isDominatedConfirm = true;
                Debug.Log("거점 등록");
            }
            //able 모두 삭제
            DeleteAble(FindObject);

            if (GameObject.Find("GameCores") != null)
            {
                GameObject.FindWithTag("GameManager").GetComponent<InGameSceneManager>().SendTerrainIndex(myPlanet.GetComponent<AllMeshController>().PickContainer.ToArray());
                Debug.Log("SEND : 행성 픽킹로케이션");
            }

            FindObject.GetComponent<MeshController>().isFixed = true;
            FindObject.GetComponent<MeshController>().isMine = true; // 내가 픽했던 메시들 fixed로 고정
            FindObject.GetComponent<MeshController>().isLandingSign = true;

            /////// Event 카드를 위한 매시 묶기
            myPlanet.GetComponent<AllMeshController>().ArrangeLinkedMesh(
                FindObject.GetComponent<MeshController>().name,
                FindObject.GetComponent<MeshController>().terrainstate);

        }

        // 거점등록이 확정됐으면 effect추가
        if (isDominatedConfirm == true && bDomination == false)
        {
            GameObject flagMesh = myPlanet.GetComponent<AllMeshController>().myFlag;
            GameObject effectObj = myPlanet.GetComponent<AllMeshController>().EffectObj[0];
            GameObject buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[21];
            Camera.main.GetComponent<CameraShake>().ShakeOnce();
            Destroy(flagMesh.GetComponent<MeshController>().TargetObject);
            flagMesh.GetComponent<MeshController>().EulerRotCal(flagMesh, buildingObj, 1.01f);
            flagMesh.GetComponent<MeshController>().EulerRotCalEffect(flagMesh, effectObj, 1.01f);
            bDomination = true;
        }

        FlagSetting(); // Flag검사 
        myPlanet.GetComponent<AllMeshController>().PickContainer.Clear(); // 컨테이너는 초기화
        CameraController.Once = false;
    }
    public void DeleteAble(GameObject CheckObject)
    {
        //able 모두 삭제
        for (int j = 0; j < 3; j++)
        {
            bool temp = false;

            if (CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE)
            {
                if (isDominatedCheck == false)
                {
                    for (int k = 0; k < myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; k++)
                    {
                        if (myPlanet.GetComponent<AllMeshController>().myFlag ||
                            myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k] == CheckObject)
                        {
                            CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                            temp = true;
                            break;
                        }
                    }
                }
                if (!temp)
                {
                    CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                    CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");

                    if (CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().beforeRender != null)
                    {
                        CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().beforeRender;
                        CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().beforeRender = null;
                    }
                }
            }
        }
    }

    // Setting의 역할을 바꿔줘야 함!
    // 전제1 : 플레이어는 처음 하나의 Flag를 선택할 수 밖에 없다 -> 한턴이 돌면 Flag는 정해지게 됨.
    // 정해지면?
    // 1. 선택받지 못받은 애들 
    //      - flag들은 모두 정리해야 함.
    // 2. 선택받은 애들
    //      - 선택받은 애들은 Alter_B로 바꿔줘야 함.
    //      - 선택받은 애들은 추후 점령 됐는지 확인할 수 있어야 함.
    // 3. 점령이 됐다면
    //      - Alter_C로 바꿔줘야 함 (캐릭터bit에 따라서 색상변경)
    //      - 점령 두 곳이 다 됐으면 더이상 이 로직은 무의미
    public void FlagSetting()
    {
        int tempint = 0;

        if (bDominateEnemyFlag == true && bDominateMyFlag == true) return; // 더이상 무의미한 로직이 됨

        if (!bSelectionFlag)
        {
            for (int i = 0; i < myPlanet.GetComponent<AllMeshController>().FlagContainer.Count; i++)
            {
                if (myPlanet.GetComponent<AllMeshController>().FlagContainer[i].GetComponent<MeshController>().isFixed)
                {
                    GameObject buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[21];
                    GameObject meshObj = myPlanet.GetComponent<AllMeshController>().FlagContainer[i];

                    Destroy(meshObj.GetComponent<MeshController>().TargetObject);
                    meshObj.GetComponent<MeshController>().EulerRotCalAltar(meshObj, buildingObj, 1.02f);
                    tempint++;
                }

                if (tempint == 2)
                {
                    for (int j = 0; j < myPlanet.GetComponent<AllMeshController>().FlagContainer.Count; j++)
                    {
                        if (!myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().isFixed)
                        {
                            myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().isFlag = false;
                            myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().priorState = Terrain.DEFAULT;
                            myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_DEFAULT");
                            Destroy(myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().TargetObject);
                            myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().setDefault();


                        } else
                        {
                            if (myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().currentIdentify == Identify.ENEMY)
                            {
                                enemyFlag = myPlanet.GetComponent<AllMeshController>().FlagContainer[j];
                            }
                            else
                            {
                                myFlag = myPlanet.GetComponent<AllMeshController>().FlagContainer[j];
                            }
                        }
                    }
                    bSelectionFlag = true;
                }
            }
        }
        else // Flag가 결정된 후
        {
            GameObject buildingObj = new GameObject();
            int detectedCount = 0;
            //EnemyFlag
            if (!bDominateEnemyFlag)
            {
                for (int i = 0; i < enemyFlag.GetComponent<MeshController>().NearMesh.Count; i++)
                {
                    if (enemyFlag.GetComponent<MeshController>().NearMesh[i].GetComponent<MeshController>().isFixed) // Fixed됐다면?
                    {
                        detectedCount++;
                    }
                    if (detectedCount == 12)
                    {
                        if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex >= 0 &&
                            networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex <= 3)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[22];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex == 4)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[23];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex == 5)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[24];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex == 6)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[25];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex == 7)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[26];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex == 8)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[27];
                        }
                        else
                        {
                            Debug.Log(networkSystem.GetComponent<NetworkManager>().enemyCharacterIndex);
                        }

                        Destroy(enemyFlag.GetComponent<MeshController>().TargetObject);
                        enemyFlag.GetComponent<MeshController>().EulerRotCal(enemyFlag, buildingObj, 1.0f);
                        bDominateEnemyFlag = true;

                        gameObject.GetComponent<CameraShake>().ShakeOnce();
                        //StartCoroutine(gameObject.GetComponent<CameraController>().movePositionEffect(buildingObj.transform.position));
                    }
                }
            }

            detectedCount = 0;
            // MyFlag
            if (!bDominateMyFlag)
            {
                for (int i = 0; i < myFlag.GetComponent<MeshController>().NearMesh.Count; i++)
                {
                    if (myFlag.GetComponent<MeshController>().NearMesh[i].GetComponent<MeshController>().isFixed) // Fixed됐다면?
                    {
                        detectedCount++;
                    }
                    if (detectedCount == 12)
                    {
                        if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex >= 0 &&
                            networkSystem.GetComponent<NetworkManager>().playerCharacterIndex <= 3)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[22];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex == 4)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[23];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex == 5)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[24];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex == 6)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[25];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex == 7)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[26];
                        }
                        else if (networkSystem.GetComponent<NetworkManager>().playerCharacterIndex == 8)
                        {
                            buildingObj = myPlanet.GetComponent<AllMeshController>().buildingObj[27];
                        }
                        else
                        {
                            Debug.Log(networkSystem.GetComponent<NetworkManager>().playerCharacterIndex);
                        }

                        Destroy(myFlag.GetComponent<MeshController>().TargetObject);
                        myFlag.GetComponent<MeshController>().EulerRotCal(myFlag, buildingObj, 1.0f);
                        bDominateMyFlag = true;
                    }
                }
            }
        }
    }
}