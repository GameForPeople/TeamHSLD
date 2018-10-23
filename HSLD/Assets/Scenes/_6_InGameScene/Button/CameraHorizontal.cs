using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraHorizontal : MonoBehaviour {
    public Camera mainCamera;
    public GameObject MyPlanet;
    public Transform MyPlanet_Pos;
    public float RotationSensitivity;

    // Use this for initialization
    void Start () {
        //MyPlanet = GameObject.Find("Sphere_320Objects_40X");
        //mainCamera = Camera.main;
    }
    
    void Update()
    {
        //mainCamera.transform.RotateAround(MyPlanet.transform.position, Vector3.left, RotationSensitivity);
    }

    public void OnTrigger()
    {
        mainCamera.transform.RotateAround(MyPlanet.transform.position, Vector3.right, RotationSensitivity);

    }
}
