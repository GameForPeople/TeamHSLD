﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PCverPIcking : MonoBehaviour
{
    public Camera mainCamera;
    public GameObject myPlanet;
    private GameObject PickedMeshObj;
    RaycastHit hit;

    private void Start()
    {
        myPlanet = GameObject.Find("Sphere_320Objects_40X");
    }

    private void Update()
    {
        int Length = myPlanet.GetComponent<AllMeshController>().PickContainer.Count;

        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow != FLOW.TO_PICKINGLOC)
            return;

        if (CameraController.TurnChange) // 턴체인지가 true로 들어옴
        {
            //for (int i = 0;i < myPlanet.GetComponent<AllMeshController>().PickContainer.Length; i++)
            //{
            //    myPlanet.GetComponent<AllMeshController>().PickContainer[i] = 0;
            //}
            myPlanet.GetComponent<AllMeshController>().PickContainer.Clear();
            CameraController.ChangeableCount = DiceSystem.getDiceNum;
            
            Debug.Log("내 턴으로 돌아왔을 때 한 번 : " + CameraController.ChangeableCount);

            //CameraController.Once = true;

            //////////////////////////////////////////
            CameraController.TurnChange = false;
        } 

        if (AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn != TURN.MYTURN_NOTYETFLAG == true) // 내 턴 아님
        {
            if (CameraController.Once == true) // 내 턴에서 넘어갈 때 한번만
            {
                Debug.Log("내 턴에서 넘어갈 때 한번만");
                for (int i = 0; i < Length; i++)
                {
                    GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());

                    //able 모두 삭제
                    for (int j = 0; j < 3; j++)
                    {
                        if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE)
                        {
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                        }
                    }

                    FindObject.GetComponent<MeshController>().isFixed = true;
                    FindObject.GetComponent<MeshController>().isMine = true; // 내가 픽했던 메시들 fixed로 고정
                }
                myPlanet.GetComponent<AllMeshController>().PickContainer.Clear(); // 컨테이너는 초기화
                CameraController.TurnChange = true;
                CameraController.Once = false;
            }
        }
        else // 내 턴임
        {
            if (CameraController.Once == false) // 내 턴으로 돌아왔을 떄 한 번만
            {
                Debug.Log("내 턴으로 돌아왔을 때 한 번만");
                CameraController.Once = true;
                CameraController.TurnChange = true;
            }

            if (Input.GetMouseButton(0) && CameraController.offset < 0.5)
            {
                Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
                RaycastHit hitObj;

                if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
                {
                    PickedMeshObj = hitObj.transform.gameObject;

                    if (PickedMeshObj.GetComponent<MeshController>() == null)
                    {
                        PickedMeshObj.AddComponent<MeshController>();
                    }

                    if (!PickedMeshObj.GetComponent<MeshController>().isFixed)
                    {
                        if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                        {

                            if (myPlanet.GetComponent<AllMeshController>().isEmpty()) // 비어있을 때 로직
                            {
                                Debug.Log("비어있어!");
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                myPlanet.GetComponent<AllMeshController>().PickContainer.Add(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                            }
                            else // 하나라도 들어있으면
                            {
                                if (PickedMeshObj.GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // DEFALT는 더이상 들어가면 안됨.
                                    return;

                                if (myPlanet.GetComponent<AllMeshController>().PickContainer.Contains(PickedMeshObj.GetComponent<MeshController>().MeshNumber))
                                {
                                    // 다시 누른 거
                                    if (myPlanet.GetComponent<AllMeshController>().PickContainer[0] == PickedMeshObj.GetComponent<MeshController>().MeshNumber ||
                                        myPlanet.GetComponent<AllMeshController>().PickContainer[myPlanet.GetComponent<AllMeshController>().PickContainer.Count-1]
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
                                else
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
                                                Debug.Log("앞 쪽 꼬리에 들어갑니다.");
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
        }
    }

}
