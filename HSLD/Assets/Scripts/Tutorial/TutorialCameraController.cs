using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialCameraController : MonoBehaviour {
    private Transform MyPlanet;
    public Camera mainCamera;
    private Vector2 rotateAmount;

    public float orthoZoomSpeed;
    public float minDistance;
    public float maxDistance;
    public float RotationSensitivity;
    private Transform myTransform;
    private Vector3 priorPosition;
    public static float offsetTutorial;

    public static int DiceCount;
    public static int ChangeableCount;

    public bool myTurn;
    public static bool Once;
    public static bool TurnChange;
    public static int lastmesh;

    private float HorizontalValue;
    private float VerticalVelue;

    // 임시
    public float speed;

    void Start()
    {
        MyPlanet = GameObject.FindWithTag("InGamePlanet").transform;

        myTransform = GetComponent<Transform>();
        myTurn = true;
        Once = true;
        TurnChange = true;
        mainCamera.farClipPlane = 150;
    }

    public void TurnVertical(bool isUp)
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;

        if (isUp)
        {
            rotateAmount.x -= 15;

            myTransform.localRotation = Quaternion.Euler(rotateAmount.y, rotateAmount.x, 0);
            myTransform.localPosition = MyPlanet.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
        else
        {
            rotateAmount.x += 15;

            myTransform.localRotation = Quaternion.Euler(rotateAmount.y, rotateAmount.x, 0);
            myTransform.localPosition = MyPlanet.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
    }

    public void TurnHorizontal(bool isRight)
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;

        if (isRight)
        {
            rotateAmount.y += 15;

            myTransform.localRotation = Quaternion.Euler(rotateAmount.y, rotateAmount.x, 0);
            myTransform.localPosition = MyPlanet.position - (myTransform.rotation * Vector3.forward * fdistance);
        }
        else
        {
            rotateAmount.y -= 15;

            myTransform.localRotation = Quaternion.Euler(rotateAmount.y, rotateAmount.x, 0);
            myTransform.localPosition = MyPlanet.position - (myTransform.rotation * Vector3.forward * fdistance);
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

            if (Mathf.Abs((transform.position - end).magnitude) < 1.5f) yield break; // 보간 제한

            transform.position = Vector3.Lerp(transform.position, end, Time.deltaTime * 2.5f);

            mainCamera.transform.LookAt(MyPlanet);
            yield return null;
        }
    }
    
    void Update()
    {
        Vector3 normalDirection = myTransform.position - MyPlanet.position;
        float fdistance = normalDirection.magnitude;
        normalDirection = Vector3.Normalize(normalDirection);

        Touch[] touches = Input.touches;
        if (offsetTutorial > 0)
        {
            offsetTutorial -= 3.0f * Time.deltaTime;
        } // 피킹 관련 

        if (Input.touchCount == 1 && offsetTutorial < 0.5)
        {
            mainCamera.GetComponent<TutorialPcVerCamera>().Picked(true);
        }
    }
}
