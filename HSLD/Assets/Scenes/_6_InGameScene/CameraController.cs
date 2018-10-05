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
    private Color grayColor;
    private Color seaColor;
    private Color mountainColor;
    private Color moderationColor;
    private Color barrenColor;
    private Color coldColor;
    private Color32 unknownColor;
    private float offset;

    private Material TestMaterial;
    public int ChangeableCount;
    public int DiceCount = 10;

    void Start()
    {
        myTransform = GetComponent<Transform>();

        //myColor = new Color(Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f));
        //unknownColor = new Color(255,255,255,255);
        //grayColor = new Color32(150, 150, 150, 255);
        //seaColor = new Color32(156, 227, 221, 255);
        //mountainColor = new Color32(60, 150, 115, 255);
        //moderationColor = new Color32(141, 212, 108, 255);
        //coldColor = new Color32(213, 228, 231, 255);
        //barrenColor = new Color32(206, 154, 143, 255);

        ChangeableCount = DiceCount - 1;
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
                        if (Input.GetTouch(0).phase == TouchPhase.Moved)
                        {
                            if (!PickedMeshObj.GetComponent<MeshController>().isFixed) // 정해져있지 않음, 턴이 지나면 Fixed로 바꿔주는 게 필요
                            {
                                PickedMeshObj.GetComponent<MeshController>().isAwake = true; // 깨어나면 계산 후 다시 잠듦
                                
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
        }
    }
}