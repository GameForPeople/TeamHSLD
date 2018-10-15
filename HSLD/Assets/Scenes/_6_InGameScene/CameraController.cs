using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Transform MyPlanet;
    public Camera mainCamera;
    private Vector2 PrevPoint;

    public float orthoZoomSpeed;
    public float minDistance;
    public float maxDistance;
    public float RotationSensitivity;
    private Transform myTransform;
    private Vector3 priorPosition;
    public static GameObject PickedMeshObj;
    public static float offset;
    
    public static int DiceCount;
    public static int ChangeableCount;

    public bool myTurn;
    public static bool Once;
    public static bool TurnChange;
    
    void Start()
    {
        myTransform = GetComponent<Transform>();
        myTurn = true;
        Once = true;
        TurnChange = true;
    }

    public void TurnVertical()
    {
        mainCamera.transform.RotateAround(MyPlanet.transform.position, Vector3.left, 20);
    }

    public void TurnHorizontal()
    {
        mainCamera.transform.RotateAround(MyPlanet.transform.position, Vector3.up, 20);
    }

    void Update()
    {

        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;
        normalDirection = Vector3.Normalize(normalDirection);

        Touch[] touches = Input.touches;

        if (mainCamera)
        {
            if (offset > 0)
            {
                offset -= 0.1f;
            } // 피킹 관련 

            if (fdistance >= minDistance || fdistance <= maxDistance)
            {
                myTransform.position = priorPosition;
            } // 확대축소 관련

            if (Input.touchCount == 2) // 확대축소 스와이프
            {
                Touch touchZero = Input.GetTouch(0);
                Touch touchOne = Input.GetTouch(1);

                Vector2 touchZeroPrevPos = touchZero.position - touchZero.deltaPosition;
                Vector2 touchOnePrevPos = touchOne.position - touchOne.deltaPosition;

                float prevTouchDeltaMag = (touchZeroPrevPos - touchOnePrevPos).magnitude;
                float touchDeltaMag = (touchZero.position - touchOne.position).magnitude;

                float deltaMagnitudediff = prevTouchDeltaMag - touchDeltaMag;

                if (fdistance < minDistance && fdistance > maxDistance)
                {
                    priorPosition = myTransform.position;
                    myTransform.position = myTransform.position - -(normalDirection * deltaMagnitudediff * orthoZoomSpeed);
                }

                if (Input.GetTouch(0).phase == TouchPhase.Moved)
                {

                    PrevPoint = Input.GetTouch(0).position - Input.GetTouch(0).deltaPosition;

                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.left,
                        (Input.GetTouch(0).position.y - PrevPoint.y) * 0.5f * RotationSensitivity);

                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.up,
                        (Input.GetTouch(0).position.x - PrevPoint.x) * RotationSensitivity);

                    PrevPoint = Input.GetTouch(0).position;
                }
                mainCamera.transform.LookAt(MyPlanet);
            }

            //if (Once == false && AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn != TURN.MYTURN) // 상대 턴으로 갈 때 한번만
            //{
            //    for (int i = 0; i < AllMeshController.PickContainer.Length; i++)
            //    {
            //        if (AllMeshController.PickContainer[i] != 0)
            //        {
            //            GameObject.Find(AllMeshController.PickContainer[i].ToString()).GetComponent<MeshController>().isFixed = true;
            //            GameObject.Find(AllMeshController.PickContainer[i].ToString()).GetComponent<MeshController>().isMine = true;
            //        } // 내가 픽했던 메시들 fixed로 고정
            //    }
            //    AllMeshController.PickContainer.Initialize(); // 컨테이너는 초기화
            //                                // 
            //}
            //DiceCount = DiceSystem.getDiceNum;

            //if (Input.touchCount == 1)
            //{
            //    RaycastHit hit;

            //    Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

            //    if (Physics.Raycast(ray, out hit))
            //    {
            //        //PickedMeshObj = GameObject.Find(hit.transform.name);
            //        // [NEW] 내 턴 아닐 때는 어떤 정보볼 수 있을 지 정해야 함.
            //    }
            //}

            ////Once = true;

            //// myTurn을 내턴일 때는 받아와야 돼

            //if (AllMeshController.IngameManager.GetComponent<TurnSystem>().currentTurn != TURN.MYTURN) // 내 턴이 아니면 리턴해
            //{   
            //    return;
            //}

            ////if (Once == true) // 내 턴으로 돌아왔을 떄 한 번만
            ////{
            ////    AllMeshController.PickContainer.Initialize();
            ////    DiceCount = DiceSystem.getDiceNum + 1; // DiceSystem.getDiceNum;
            ////    ChangeableCount = DiceCount;

            ////    Once = false;
            ////}

            //// 내 턴일 때
            //if (Input.touchCount == 1 && offset < 0.5)
            //{
            //    RaycastHit hitObj;

            //    Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

            //    if (Physics.Raycast(ray, out hitObj))
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
            //                for (int i = 0; i < AllMeshController.PickContainer.Length; i++)
            //                {
            //                    if (AllMeshController.PickContainer[i] == 0 && temp == -1) // 들어가야할 위치
            //                    {
            //                        temp = i;
            //                    }
            //                    if (AllMeshController.PickContainer[i] == PickedMeshObj.GetComponent<MeshController>().MeshNumber)
            //                    {
            //                        PickedMeshObj.GetComponent<MeshController>().isAwake = true;
            //                        AllMeshController.PickContainer[i] = 0;
            //                        offset = 2;
            //                        break;
            //                    } // 이미 들어있다면 무시
            //                    if (i == AllMeshController.PickContainer.Length - 1)
            //                    {
            //                        if (temp != -1)
            //                        {
            //                            AllMeshController.PickContainer[temp] = PickedMeshObj.GetComponent<MeshController>().MeshNumber;
            //                            PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
            //                            offset = 2;

            //                            break;
            //                        }
            //                    }
            //                }
            //            }
            //        }
            //    }
            //    Once = false;
            //}
        }
    }
}