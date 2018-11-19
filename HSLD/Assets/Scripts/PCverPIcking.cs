using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PCverPIcking : MonoBehaviour
{
    public Camera mainCamera;
    public GameObject myPlanet;
    private GameObject PickedMeshObj;
    public static bool isGetFlag;
    public static bool isCheck;
    RaycastHit hit;
    public CameraShake cameraShake;
    public float duration;
    public float magnitude;

    private void Start()
    {
        myPlanet = GameObject.Find("Sphere_320Objects_40X");
        isGetFlag = false;
        isCheck = false;
    }

    private void Update()
    {
        int Length = myPlanet.GetComponent<AllMeshController>().PickContainer.Count;

        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow == FLOW.TO_PICKINGCARD)
        {
            myPlanet.GetComponent<AllMeshController>().PickContainer.Clear();
            CameraController.ChangeableCount = DiceSystem.getDiceNum;

            //Debug.Log("ChangeableCount : " + CameraController.ChangeableCount);

        } // 카드 피킹할 때 주사위 값 받아오기

        if (TurnSystem.isSetTerrainDone)
        {
            //Debug.Log("Enemy Turn");
            if (CameraController.Once == true) // 내 턴에서 넘어갈 때 한번만
            {
                TurnChangeLogic();
            }
        }

        if (Input.GetMouseButton(0) && CameraController.offset < 0.5)
        {
            Picked();
        }
    }

    public void Picked()
    {
        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow != FLOW.TO_PICKINGLOC ||
            AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn == TURN.ENEMYTURN)
            return;

        Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
        RaycastHit hitObj;

        if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
        {
            PickedMeshObj = hitObj.transform.gameObject;

            //// CameraShake Test Code
            //if (Input.GetMouseButton(1))
            //{
            //    if (!CameraShake.isCameraShake)
            //    {
            //        StartCoroutine(cameraShake.Shake(duration, magnitude, PickedMeshObj.GetComponent<MeshController>().Linkednum));
            //        // Linkednum != 0 && isMine == false 일 때 이벤트 카드
            //    }
            //    CameraController.offset = 2;
            //    return;
            //}

            if (PickedMeshObj.GetComponent<MeshController>() == null)
            {
                PickedMeshObj.AddComponent<MeshController>();
            }

            if (!PickedMeshObj.GetComponent<MeshController>().isFixed)
            {
                if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                {

                    if (myPlanet.GetComponent<AllMeshController>().isEmpty()) // 턴 시작후 첫번째 로직
                    {
                        //깃발 획득 후에는 어디서든 피킹 시작 가능
                        if (isGetFlag == true)
                        {
                            PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                            myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                        }
                        else // 점령전
                        {
                            if (PickedMeshObj.GetComponent<MeshController>().isFlagable == true) // 가장 처음
                            {
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);

                                // 깃발 획득한 뒤에는 Flag표시는 다 지우고 내 것만 남아
                                for (int i = 0; i < myPlanet.GetComponent<AllMeshController>().FlagContainer.Count; i++)
                                {
                                    myPlanet.GetComponent<AllMeshController>().FlagContainer[i].GetComponent<MeshController>().isFlagable = false;
                                    myPlanet.GetComponent<AllMeshController>().FlagContainer[i].GetComponent<MeshController>().TargetObject.transform.position = new Vector3(3, 3, 3);
                                    Destroy(myPlanet.GetComponent<AllMeshController>().FlagContainer[i].GetComponent<MeshController>().TargetObject);
                                    myPlanet.GetComponent<AllMeshController>().FlagContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                                }
                                myPlanet.GetComponent<AllMeshController>().FlagContainer.RemoveRange(0, 4);

                                myPlanet.GetComponent<AllMeshController>().FlagContainer.Add(PickedMeshObj);
                                PickedMeshObj.GetComponent<Renderer>().material = Resources.Load<Material>("M_FlagAble");
                                PickedMeshObj.GetComponent<MeshController>().EulerRotCal(PickedMeshObj, myPlanet.GetComponent<AllMeshController>().buildingObj[0], 1.03f); // 내 색 오브젝트 필요할듯?
                            }
                            else // 깃발 획득했지만, 아직 점령 전일 때
                            {
                                for (int i = 0; i < myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count; i++)
                                {
                                    if (myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[i] == PickedMeshObj)
                                    {
                                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                        myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
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
                                == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
                            {
                                // 첫 값이 혹은 마지막 값이야
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

                CameraController.offset = 2;
            }
        }
        CameraController.Once = true;
    }

    public void TurnChangeLogic()
    {
        int Length = myPlanet.GetComponent<AllMeshController>().PickContainer.Count;

        AllMeshController.giveLinkNum++;
        Debug.Log("내 턴에서 넘어갈 때 한번만");
        for (int i = 0; i < Length; i++) // 링크드넘버 세팅
        {
            GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());
            FindObject.GetComponent<MeshController>().Linkednum = AllMeshController.giveLinkNum;
        }

        for (int i = 0; i < Length; i++)
        {
            GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());

            if (isGetFlag == true)
            {
                isCheck = true;
                Debug.Log("거점 등록");
            }
            //able 모두 삭제
            for (int j = 0; j < 3; j++)
            {
                bool temp = false;

                if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE)
                {
                    if (isCheck == false)
                    {
                        for (int k = 0; k < myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count; k++)
                        {
                            if (myPlanet.GetComponent<AllMeshController>().FlagContainer[0] ||
                                myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k] == FindObject)
                            {
                                FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                                temp = true;
                                break;
                            }
                        }
                    }
                    if (temp == false)
                    {
                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                    }
                }
            }

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

        myPlanet.GetComponent<AllMeshController>().PickContainer.Clear(); // 컨테이너는 초기화
        CameraController.Once = false;
    }

}