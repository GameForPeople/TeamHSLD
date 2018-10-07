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
        // 마우스 클릭
        if (Input.GetMouseButton(0))
        {
            Vector3 mos = Input.mousePosition;
            mos.z = mainCamera.farClipPlane;

            Vector3 dir = mainCamera.ScreenToWorldPoint(mos);

            if (Physics.Raycast(transform.position, dir, out hit, mos.z))
            {
                PickedMeshObj = GameObject.Find(hit.transform.name);
                Debug.Log(PickedMeshObj);

                if (PickedMeshObj.GetComponent<MeshController>() == null)
                    PickedMeshObj.AddComponent<MeshController>();

                // 정해져있지 않음, 턴이 지나면 Fixed로 바꿔주는 게 필요
                if (!PickedMeshObj.GetComponent<MeshController>().isFixed /*&& CameraController.offset < 1*/)
                {
                    if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                    {
                        Debug.Log("거기" + PickedMeshObj.GetComponent<MeshController>().MeshNumber);
                        PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                        //CameraController.offset = 3;
                    }
                }
                else
                    Debug.Log("요기");
            }
        }
    }

}
