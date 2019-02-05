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
    public static float offset;
    
    public static int DiceCount;
    public static int ChangeableCount;

    public bool myTurn;
    public static bool Once;
    public static bool TurnChange;
    public static int lastmesh;

    private float HorizontalValue;
    private float VerticalVelue;

    void Start()
    {
        myTransform = GetComponent<Transform>();
        myTurn = true;
        Once = true;
        TurnChange = true;
    }

    public void TurnVertical(bool isUp)
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;

        VerticalVelue += 15;
        if (isUp)
        {
            myTransform.localRotation = Quaternion.Euler(myTransform.localRotation.y + HorizontalValue, myTransform.localRotation.x + VerticalVelue, 0);
            myTransform.localPosition = MyPlanet.transform.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
        else
        {
            myTransform.localRotation = Quaternion.Euler(myTransform.localRotation.y + HorizontalValue, myTransform.localRotation.x - VerticalVelue, 0);
            myTransform.localPosition = MyPlanet.transform.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
    }

    public void TurnHorizontal(bool isRight)
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;

        HorizontalValue += 15;

        if (isRight)
        {
            myTransform.localRotation = Quaternion.Euler(myTransform.localRotation.y - HorizontalValue, myTransform.localRotation.x + VerticalVelue, 0);
            myTransform.localPosition = MyPlanet.transform.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
        else
        {
            myTransform.localRotation = Quaternion.Euler(myTransform.localRotation.y + HorizontalValue, myTransform.localRotation.x + VerticalVelue, 0);
            myTransform.localPosition = MyPlanet.transform.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
    }

    public IEnumerator movePosition(Vector3 end)
    {
        int enterMesh = lastmesh;
        while (true)
        {
            if (lastmesh != enterMesh)
            {
                yield break; // 선택 매쉬가 달라졌으면 off
            }
            
            if (Mathf.Abs((transform.position - end).magnitude) < 1.0f) yield break; // 보간 제한
            

            transform.position = Vector3.Lerp(transform.position, end, Time.deltaTime);
            Camera.main.transform.LookAt(MyPlanet);
            yield return null;
        }
    }

    void Update()
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;
        normalDirection = Vector3.Normalize(normalDirection);

        Touch[] touches = Input.touches;

        // 카메라의 기본 회전
        if (mainCamera)
        {
            if (offset > 0)
            {
                offset -= 3.0f * Time.deltaTime;
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
                    PrevPoint += (Input.GetTouch(0).position - Input.GetTouch(0).deltaPosition) * Time.deltaTime / 10000;
                    //Debug.Log(PrevPoint);

                    //Vector3 position = MyPlanet.position - (rotation * Vector3.forward * fdistance);

                    myTransform.localRotation = Quaternion.Euler((Input.GetTouch(0).position.y - PrevPoint.y), 
                        (Input.GetTouch(0).position.x - PrevPoint.x), 0);
                    myTransform.localPosition = MyPlanet.position - (myTransform.rotation * Vector3.forward * fdistance);

                    /*
                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.left,
                        (Input.GetTouch(0).position.y - PrevPoint.y) );

                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.up,
                        (Input.GetTouch(0).position.x - PrevPoint.x) * RotationSensitivity);
                    */
                    //PrevPoint = Input.GetTouch(0).position;
                }
            }
            //PrevPoint.Set(0, 0);
            myTransform.LookAt(MyPlanet);
        }

        if (Input.touchCount == 1 && CameraController.offset < 0.5)
        {
            Camera.main.GetComponent<PCverPIcking>().Picked(true);
        }

        if (Input.touchCount == 4 && CameraController.offset < 0.5)
        {
            MyPlanet.GetComponent<AllMeshController>().CleanPickContainer(); 
        }
    }
}