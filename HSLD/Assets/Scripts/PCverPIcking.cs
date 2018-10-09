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
        if (Input.GetMouseButton(0) && CameraController.offset < 1)
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

                //정해져있지 않음, 턴이 지나면 Fixed로 바꿔주는 게 필요
                if (!PickedMeshObj.GetComponent<MeshController>().isFixed)
                {
                    if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                    {
                        int temp = -1;
                        for (int i = 0; i < CameraController.PickContainer.Length; i++)
                        {
                            if (CameraController.PickContainer[i] == 0 && temp == -1) // 들어가야할 위치
                            {
                                temp = i;
                            }
                            if (CameraController.PickContainer[i] == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
                            {
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true;
                                CameraController.PickContainer[i] = 0;
                                CameraController.offset = 3;
                                break;
                            } // 이미 들어있다면 무시
                            if (i == CameraController.PickContainer.Length - 1)
                            {
                                if (temp != -1)
                                {
                                    CameraController.PickContainer[temp] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
                                    PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                                    CameraController.offset = 3;

                                    //Debug.Log(CameraController.PickContainer);

                                    for (int j = 0; j < CameraController.PickContainer.Length; j++)
                                    {
                                        Debug.Log(CameraController.PickContainer[j]);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}
