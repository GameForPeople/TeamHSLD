﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialPcVerCamera : MonoBehaviour
{
    public Camera mainCamera;
    private GameObject myPlanet;
    private GameObject PickedMeshObj;
    public static bool isDominatedCheck; // 턴이 넘어가지 않아 아직 미정인 상태
    public static bool isDominatedConfirm; // 턴이 넘어가서 확정된 상태
    RaycastHit hit;
    public CameraShake cameraShake;
    public float duration;
    public float magnitude;
    private bool once;

    private TutorialFlowSystem flowSystem;

    private void Awake()
    {
        myPlanet = GameObject.FindWithTag("InGamePlanet");

        isDominatedCheck = false;
        isDominatedConfirm = false;

        flowSystem = GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>();
    }

    private void Update()
    {

        int Length = myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Count;

        if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGCARD))
        {
            myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Clear();
            CameraController.ChangeableCount = CameraController.DiceCount;

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
        if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGEVENTCARDLOC))
        {
            if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
            {
                PickedMeshObj = hitObj.transform.gameObject;

                switch (EventCardManager.selectedIndex)
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

                TutorialAllMeshController.IngameManager.GetComponent<EventCardManager>().PickLocDone(PickedMeshObj, PickedMeshObj.GetComponent<MeshController>().terrainstate);

            }
        }

        if (flowSystem.currentFlow != FLOW.TO_PICKINGLOC)
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
                if (myPlanet.GetComponent<TutorialAllMeshController>().isEmpty()) // 턴 시작후 첫번째 로직
                {
                    //점령이 끝난 뒤 첫턴 로직 (다른 곳도 선택할 수 있도록)
                    if (isDominatedConfirm == true)
                    {
                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                        myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                    }
                    else // 점령전 (내 Flag와 내 flagable만 선택할 수 있어야 함.)
                    {
                        if (myPlanet.GetComponent<TutorialAllMeshController>().myFlag == null && PickedMeshObj.GetComponent<MeshController>().isFlag == true) // Flag하나만 선택할 수 있어야 해.
                        {
                            PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                            myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                            myPlanet.GetComponent<TutorialAllMeshController>().myFlag = PickedMeshObj;
                        }
                        else // 깃발 획득했지만, 아직 점령 전일 때 (내 NearMesh를 선택할 수 있어야 해.)
                        {
                            if (myPlanet.GetComponent<TutorialAllMeshController>().myFlag != null)
                            {
                                for (int i = 0; i < myPlanet.GetComponent<TutorialAllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; i++)
                                {
                                    if (myPlanet.GetComponent<TutorialAllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[i] == PickedMeshObj)
                                    {
                                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                        myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
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
                    if (myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Contains(PickedMeshObj.GetComponent<MeshController>().MeshNumber))
                    {
                        // 다시 누른 거
                        if (myPlanet.GetComponent<TutorialAllMeshController>().PickContainer[0] == PickedMeshObj.GetComponent<MeshController>().MeshNumber ||
                            myPlanet.GetComponent<TutorialAllMeshController>().PickContainer[myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Count - 1]
                            == PickedMeshObj.GetComponent<MeshController>().MeshNumber) // 첫 값 혹은 마지막 값이야
                        {
                            if (CameraController.ChangeableCount < CameraController.DiceCount + 1)
                            {
                                myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Remove(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
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
                                if (GameObject.Find(myPlanet.GetComponent<TutorialAllMeshController>().PickContainer[0].ToString()).GetComponent<MeshController>().JointMesh[i].name
                                    == PickedMeshObj.GetComponent<MeshController>().MeshNumber.ToString())
                                {
                                    //Debug.Log("앞 쪽 꼬리에 들어갑니다.");
                                    myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Insert(0, PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                                    PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                    temp = true;
                                    break;
                                }
                            }
                            if (temp == false) // 0번에 있는 애가 아니면 그냥 뒤에 붙어
                            {
                                myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber); // 값을 추가
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
        int Length = myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Count;
        TutorialAllMeshController.giveLinkNum++;
        //Debug.Log("내 턴에서 넘어갈 때 한번만");
        for (int i = 0; i < Length; i++) // 링크드넘버 세팅
        {
            GameObject FindObject = GameObject.Find(TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().PickContainer[i].ToString());
            FindObject.GetComponent<MeshController>().LinkedNumber = TutorialAllMeshController.giveLinkNum;
        }

        for (int i = 0; i < Length; i++)
        {
            GameObject FindObject = GameObject.Find(TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().PickContainer[i].ToString());

            if (isDominatedCheck == true)
            {
                isDominatedConfirm = true;
            }
            //able 모두 삭제
            DeleteAble(FindObject);

            FindObject.GetComponent<MeshController>().isFixed = true;
            FindObject.GetComponent<MeshController>().isMine = true; // 내가 픽했던 메시들 fixed로 고정
            FindObject.GetComponent<MeshController>().isLandingSign = true;

            /////// Event 카드를 위한 매시 묶기
            myPlanet.GetComponent<TutorialAllMeshController>().ArrangeLinkedMesh(
                FindObject.GetComponent<MeshController>().name,
                FindObject.GetComponent<MeshController>().terrainstate);

        }

        // 거점등록이 확정됐으면 effect추가
        if (isDominatedConfirm == true && once == false)
        {
            GameObject flagObj = myPlanet.GetComponent<TutorialAllMeshController>().myFlag;
            GameObject effectObj = myPlanet.GetComponent<TutorialAllMeshController>().EffectObj[0];
            Camera.main.GetComponent<CameraShake>().ShakeOnce();
            flagObj.GetComponent<MeshController>().EulerRotCalEffect(flagObj, effectObj, 1.01f);
            once = true;
        }

        FlagSetting(); // Flag검사 
        myPlanet.GetComponent<TutorialAllMeshController>().PickContainer.Clear(); // 컨테이너는 초기화
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
                    for (int k = 0; k < myPlanet.GetComponent<TutorialAllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; k++)
                    {
                        if (myPlanet.GetComponent<TutorialAllMeshController>().myFlag ||
                            myPlanet.GetComponent<TutorialAllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k] == CheckObject)
                        {
                            CheckObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                            temp = true;
                            break;
                        }
                    }
                }
                if (temp == false)
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

    public void FlagSetting()
    {
        // 이미 다 획득한 상황에 대한 값이 있고 return시켜. [낭비]

        int tempint = 0;

        //깃발 획득한 뒤에는 Flag표시는 다 지우고 내 것만 남아
        for (int i = 0; i < myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer.Count; i++)
        {
            if (myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer[i].GetComponent<MeshController>().isFixed)
            {
                tempint++;
            }

            if (tempint == 2)
            {
                for (int j = 0; j < myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer.Count; j++)
                {
                    if (!myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer[j].GetComponent<MeshController>().isFixed)
                    {
                        myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer[j].GetComponent<MeshController>().isFlag = false;
                        Destroy(myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer[j].GetComponent<MeshController>().TargetObject);
                        myPlanet.GetComponent<TutorialAllMeshController>().FlagContainer[j].GetComponent<MeshController>().setDefault();

                    }
                }
            }
        }
    }
}
