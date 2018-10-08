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
        if (Input.GetMouseButton(0))
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
                if (!PickedMeshObj.GetComponent<MeshController>().isFixed && CameraController.offset < 1)
                {
                    if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                    {
                        Debug.Log(PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                        PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                        CameraController.offset = 3;
                    }
                }
            }
        }
    }

}
