using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PCverPIcking : MonoBehaviour
{
    public Camera mainCamera;
    private GameObject PickedMeshObj;
    RaycastHit hit;

    private void Update()
    {
        //if (Input.GetMouseButton(0) && CameraController.offset < 1)
        //{
        //    Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
        //    RaycastHit hitObj;

        //    if (Physics.Raycast(ray, out hitObj, Mathf.Infinity))
        //    {
        //        PickedMeshObj = hitObj.transform.gameObject;

        //        if (PickedMeshObj.GetComponent<MeshController>() == null)
        //        {
        //            PickedMeshObj.AddComponent<MeshController>();
        //        }

        //        if (!PickedMeshObj.GetComponent<MeshController>().isFixed)
        //        {
        //            if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
        //            {
        //                int temp = -1;
        //                for (int i = 0; i < CameraController.PickContainer.Length; i++)
        //                {
        //                    if (CameraController.PickContainer[i] == 0 && temp == -1) // 들어가야할 위치
        //                    {
        //                        temp = i;
        //                    }
        //                    if (CameraController.PickContainer[i] == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
        //                    {
        //                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
        //                        CameraController.PickContainer[i] = 0;
        //                        CameraController.offset = 3;
        //                        break;
        //                    } // 이미 들어있다면 무시
        //                    if (i == CameraController.PickContainer.Length - 1)
        //                    {
        //                        if (temp != -1)
        //                        {
        //                            CameraController.PickContainer[temp] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
        //                            PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
        //                            CameraController.offset = 3;

        //                            break;
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }
        //}

        if (AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn != TURN.MYTURN) // 내 턴이 아니면 리턴해
        {
            Debug.Log("내 턴 아니니 리턴해");
            return;
        }
        else
        {
            Debug.Log("내 턴이야");
        }

        if (CameraController.Once == true) // 내 턴으로 돌아왔을 떄 한 번만
        {
            AllMeshController.PickContainer.Initialize();
            CameraController.DiceCount = 10; // DiceSystem.getDiceNum;
            CameraController.ChangeableCount = CameraController.DiceCount - 1;

            CameraController.Once = false;
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
                        int temp = -1;
                        for (int i = 0; i < AllMeshController.PickContainer.Length; i++)
                        {
                            if (AllMeshController.PickContainer[i] == 0 && temp == -1) // 들어가야할 위치
                            {
                                temp = i;
                            }
                            if (AllMeshController.PickContainer[i] == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
                            {
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                AllMeshController.PickContainer[i] = 0;
                                CameraController.offset = 2;
                                break;
                            } // 이미 들어있다면 무시
                            if (i == AllMeshController.PickContainer.Length - 1)
                            {
                                if (temp != -1)
                                {
                                    AllMeshController.PickContainer[temp] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
                                    PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                                    CameraController.offset = 2;

                                    Debug.Log(i);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            CameraController.Once = false;
        }
    }

}
