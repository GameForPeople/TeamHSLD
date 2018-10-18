using System.Collections;
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
        int Length = myPlanet.GetComponent<AllMeshController>().PickContainer.Length;

        if (AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow != FLOW.TO_PICKINGLOC)
            return;

        if (CameraController.TurnChange) // 턴체인지가 true로 들어옴
        {
            //for (int i = 0;i < myPlanet.GetComponent<AllMeshController>().PickContainer.Length; i++)
            //{
            //    myPlanet.GetComponent<AllMeshController>().PickContainer[i] = 0;
            //}
            myPlanet.GetComponent<AllMeshController>().PickContainer.Initialize();
            CameraController.ChangeableCount = DiceSystem.getDiceNum;
            
            Debug.Log("내 턴으로 돌아왔을 때 한 번 : " + CameraController.ChangeableCount);

            //CameraController.Once = true;

            //////////////////////////////////////////
            CameraController.TurnChange = false;
        } 

        if (AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn != TURN.MYTURN == true) // 내 턴 아님
        {
            if (CameraController.Once == true) // 내 턴에서 넘어갈 때 한번만
            {
                Debug.Log("내 턴에서 넘어갈 때 한번만");
                for (int i = 0; i < Length; i++)
                {
                    if (myPlanet.GetComponent<AllMeshController>().PickContainer[i] != 0)
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
                        FindObject.GetComponent<MeshController>().isMine = true;
                    } // 내가 픽했던 메시들 fixed로 고정


                }
                myPlanet.GetComponent<AllMeshController>().PickContainer.Initialize(); // 컨테이너는 초기화
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
                                myPlanet.GetComponent<AllMeshController>().PickContainer[0] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
                                CameraController.offset = 2;
                            }
                            else // 하나라도 들어있으면
                            {
                                if (PickedMeshObj.GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // DEFALT는 더이상 들어가면 안됨.
                                    return;

                                int temp = -1;
                                for (int i = 0; i < Length; i++) // 내가 소유한 매쉬를 담은 컨테이너 순회
                                {
                                    if (myPlanet.GetComponent<AllMeshController>().PickContainer[i] == 0 && temp == -1) // 들어가야할 위치에 temp를 수정해줘
                                    {
                                        temp = i;
                                    }
                                    if (myPlanet.GetComponent<AllMeshController>().PickContainer[i] == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
                                    {
                                        if (CameraController.ChangeableCount < CameraController.DiceCount + 1)
                                        {
                                            //Debug.Log("같은 곳 두번 피킹했어");
                                            PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                            myPlanet.GetComponent<AllMeshController>().PickContainer[i] = 0;

                                            break;
                                        }
                                    } // 이미 들어있는 곳 피킹 했다면 해당 자리 초기화
                                    if (i == Length - 1)
                                    {
                                        if (temp != -1)
                                        {
                                            if (CameraController.ChangeableCount > 0)
                                            {
                                                //Debug.Log("처음보는 곳이야!!" + temp);
                                                PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                                                myPlanet.GetComponent<AllMeshController>().PickContainer[temp] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
                                                break;
                                            }
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
