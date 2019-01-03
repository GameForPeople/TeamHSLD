using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClientBaseManager : MonoBehaviour
{
    GameObject clientBaseObject;    // ClientBase_Space 변수 -> Active on/off를 위해 저장
    GameObject cameraObject;        // 카메라 오브젝트 변수 -> 트랜스폼 위해 저장

    private bool isActiveClientBaseObject;

    private Vector3 cameraPosition; 
    private float cameraAngle;
    private float cameraDistance;

    IEnumerator cameraRotate;

    // Use this for initialization
    void Start()
    {
        clientBaseObject = GameObject.Find("GameCores").transform.Find("ClientBase_Space").gameObject;
        cameraObject = GameObject.Find("GameCores").transform.Find("ClientBase_Space").transform.Find("ClientBase_Camera").gameObject;

        isActiveClientBaseObject = true;

        cameraAngle = 270.0f;   //  0, -1
        cameraDistance = 200.0f;    // Fixed
        cameraPosition = new Vector3(0, 0, -500.0f);

        cameraRotate = CameraRotateCoroutine();

        StartCoroutine("StartCameraMoveCoroutine"); // 관리 될 필요 없는 코루틴.
    }

    /*
        OnOff_ClientBaseSpace
            #0. 게임 시작, 게임 종료 등의 이유로, ClientBaseSpace의 변동사항이 있을 때 호출 필요.
            ?0. 상태 변수 bool 값을 두고, 자동으로 관리하도록 하는 것도 좋을 듯 한데...? -> MainSceneUI가 열릴 수 있는 가짓 수가 2가지임.
                - isActiveClientBaseObject 도입.
    */
    public void OnOff_ClientBaseSpace(bool InIsActive) // Default 미설정.
    {
        if(InIsActive == true)
        {
            // 이미 켜진 상태이면 저리가 임마.
            if (isActiveClientBaseObject == true)
                return;

            isActiveClientBaseObject = true;

            clientBaseObject.SetActive(true);
            StartCoroutine(cameraRotate);
        }
        else /* if(InIsActive == false) */
        {
            isActiveClientBaseObject = false;

            StopCoroutine(cameraRotate);
            clientBaseObject.SetActive(false);
        }
    }

    /*
        StartCameraMoveCoroutine
            #0. 게임을 시작하면, 2초 대기 후, cameraPosition이 0, 0, -400에서, 0,0, -200이 될때까지 Z축의 양의 방향으로 200 이동합니다.
    */
    IEnumerator StartCameraMoveCoroutine()
    {
        cameraObject.transform.position = cameraPosition;

        yield return new WaitForSeconds(1.0f);
        
        while (true)
        {
            if (cameraPosition.z >= -200.0f)
            {
                StartCoroutine(cameraRotate);
                break;
            }

            cameraPosition.z += 1.5f;
            cameraObject.transform.position = cameraPosition;

            yield return new WaitForFixedUpdate();
        }
    }

    /*
        CameraRotateCoroutine
            #0. startCameraMovecoroutine이 끝나고 나서 부터, 카메라를 공전시킵니다.
    */
    IEnumerator CameraRotateCoroutine()
    {
        Vector3 zeroVector = new Vector3(0, 0, 0);
        while (true)
        {
            cameraObject.transform.RotateAround(zeroVector, Vector3.up, 0.25f);
            yield return new WaitForFixedUpdate();
        }
    }
}
