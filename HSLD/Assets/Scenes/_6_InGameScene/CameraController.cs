using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {
    public Transform MyPlanet;
    public Camera mainCamera;
    private Vector2 PrevPoint;
    
    public float orthoZoomSpeed;
    public float minDistance;
    public float maxDistance;
    public float RotationSensitivity;
    private Transform myTransform;
    private Vector3 priorPosition;
    private GameObject PickedMesh;
    private Color myColor;
    private Color grayColor;
    private float elapsed;

    void Start()
    {
        myTransform = GetComponent<Transform>();

        myColor = new Color(Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f));
        grayColor = new Color(0.8f, 0.8f, 0.8f);
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

            if (elapsed > 0)
            {
                elapsed -= 0.1f;
            } // 피킹 관련 

            if (Input.touchCount == 1)
            {
                if (Input.GetTouch(0).phase == TouchPhase.Moved)
                {

                    PrevPoint = Input.GetTouch(0).position - Input.GetTouch(0).deltaPosition;

                    //charTarget.transform.Rotate(0, -(Input.GetTouch(0).position.x - PrevPoint.x) * cameraSensitivity, 0);


                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.right,
                        -(Input.GetTouch(0).position.y - PrevPoint.y) * 0.5f * RotationSensitivity);

                    mainCamera.transform.RotateAround(MyPlanet.position, Vector3.up,
                        (Input.GetTouch(0).position.x - PrevPoint.x) * RotationSensitivity);

                    PrevPoint = Input.GetTouch(0).position;
                }
                else
                {
                    RaycastHit hit;

                    Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

                    if (Physics.Raycast(ray, out hit))
                    {
                        PickedMesh = GameObject.Find(hit.transform.name);

                        MeshRenderer PickedRenderer = PickedMesh.GetComponent<MeshRenderer>();


                        if (elapsed < 0.5)
                        {
                            if (PickedRenderer.material.color != grayColor)
                            {
                                PickedRenderer.material.color = grayColor;
                                elapsed = 1.0f;
                            }
                            else
                            {
                                myColor = new Color(Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f));
                                PickedRenderer.material.color = myColor;
                                elapsed = 1.0f;
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
            }
        }
    }
}