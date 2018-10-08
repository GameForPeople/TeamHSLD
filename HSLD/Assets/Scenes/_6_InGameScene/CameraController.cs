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
    public GameObject PickedMeshObj;
    public static float offset;

    private Material TestMaterial;
    public int DiceCount;
    public static int ChangeableCount;

    private bool myTurn;
    private bool Once;

    void Start()
    {
        myTransform = GetComponent<Transform>();
        myTurn = true;
        Once = true;
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

        //DiceCount = GetComponent<DiceSystem>().publicDiceNum;

        Touch[] touches = Input.touches;

        if (mainCamera)
        {
            if (!myTurn)
            {
                DiceCount = DiceSystem.getDiceNum;

                if (fdistance >= minDistance || fdistance <= maxDistance)
                {
                    myTransform.position = priorPosition;
                } // 확대축소 관련

                if (offset > 0)
                {
                    offset -= 0.1f;
                } // 피킹 관련 

                if (Input.touchCount == 1)
                {
                    RaycastHit hit;

                    Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

                    if (Physics.Raycast(ray, out hit))
                    {
                        PickedMeshObj = GameObject.Find(hit.transform.name);

                        // 픽 했을 때 뭐할까?
                    }
                }

                if (Input.touchCount == 2)
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

                Once = true;

                return;
            }

            if (Once == true) // 턴 바뀌고 한 번만
            {
                DiceCount = 10; // DiceSystem.getDiceNum;
                ChangeableCount = DiceCount - 1;

                Once = false;
            }

            // 내 턴일 떄
            {
                if (fdistance >= minDistance || fdistance <= maxDistance)
                {
                    myTransform.position = priorPosition;
                } // 확대축소 관련

                if (offset > 0)
                {
                    offset -= 0.1f;
                } // 피킹 관련 

                if (Input.touchCount == 1)
                {
                    RaycastHit hit;

                    Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

                    if (Physics.Raycast(ray, out hit))
                    {
                        PickedMeshObj = GameObject.Find(hit.transform.name);

                        if (ChangeableCount > 0 && ChangeableCount < DiceCount)
                        {
                            if (Input.GetTouch(0).phase == TouchPhase.Moved && offset < 1)
                            {
                                if (!PickedMeshObj.GetComponent<MeshController>().isFixed) // 정해져있지 않음, 턴이 지나면 Fixed로 바꿔주는 게 필요
                                {
                                    if (!PickedMeshObj.GetComponent<MeshController>().isAwake)
                                    {
                                        Debug.Log("awake");
                                        PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                                        offset = 3;
                                    }
                                }
                            }
                        }
                    }
                }

                if (Input.touchCount == 2)
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

                        //charTarget.transform.Rotate(0, -(Input.GetTouch(0).position.x - PrevPoint.x) * cameraSensitivity, 0);


                        mainCamera.transform.RotateAround(MyPlanet.position, Vector3.left,
                            (Input.GetTouch(0).position.y - PrevPoint.y) * 0.5f * RotationSensitivity);

                        mainCamera.transform.RotateAround(MyPlanet.position, Vector3.up,
                            (Input.GetTouch(0).position.x - PrevPoint.x) * RotationSensitivity);

                        PrevPoint = Input.GetTouch(0).position;
                    }
                    mainCamera.transform.LookAt(MyPlanet);
                }
                Once = false;
            }
        }

    }
}