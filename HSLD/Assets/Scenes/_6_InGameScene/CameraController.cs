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
    private GameObject PickedMesh;
    private Color myColor;
    private Color grayColor;
    private Color seaColor;
    private Color mountainColor;
    private Color moderationColor;
    private Color barrenColor;
    private Color coldColor;
    private Color32 unknownColor;
    private float offset;

    private Material TestMaterial;

    void Start()
    {
        myTransform = GetComponent<Transform>();

        myColor = new Color(Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f));
        unknownColor = new Color(255,255,255,255);
        grayColor = new Color32(150, 150, 150, 255);
        seaColor = new Color32(156, 227, 221, 255);
        mountainColor = new Color32(60, 150, 115, 255);
        moderationColor = new Color32(141, 212, 108, 255);
        coldColor = new Color32(213, 228, 231, 255);
        barrenColor = new Color32(206, 154, 143, 255);
        TestMaterial = Resources.Load<Material>("M_Test");
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
                    PickedMesh = GameObject.Find(hit.transform.name);

                    MeshRenderer PickedRenderer = PickedMesh.GetComponent<MeshRenderer>();

                    if (Input.GetTouch(0).phase == TouchPhase.Moved)
                    {
                        if(PickedRenderer.material.color == seaColor)
                        {
                            //PickedRenderer.material.color = grayColor;
                            PickedRenderer.material = TestMaterial;
                            offset = 0.0f;
                        }
                        else
                        {
                            PickedRenderer.material.color = seaColor;
                        }
                        //PickedRenderer.materials.GetValue(0) = new Material sbb;
                    }

                    //if (offset < 0.5)
                    //{
                    //    if (PickedRenderer.material.color != grayColor)
                    //    {
                    //        PickedRenderer.material.color = grayColor;
                    //        offset = 1.0f;
                    //    }
                    //    else
                    //    {
                    //        myColor = new Color(Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f), Random.Range(0.3f, 1.0f));
                    //        PickedRenderer.material.color = myColor;
                    //        offset = 1.0f;
                    //    }
                    //}
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
            }
        }
    }
}