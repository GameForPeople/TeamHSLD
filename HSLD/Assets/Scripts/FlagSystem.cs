using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlagSystem : MonoBehaviour
{
    public CameraVertical cameravertical;
    public CameraHorizontal camerahorizontal;
    [HideInInspector] public GameObject myPlanet;

    private void Start()
    {
        myPlanet = GameObject.FindWithTag("InGamePlanet");
        //SpinRandom();
    }

    int randomValue()
    {
        return Random.Range(0, 360);
    }

    void SpinRandom()
    {
        cameravertical.RotationSensitivity = randomValue();
        cameravertical.OnTrigger();
        camerahorizontal.RotationSensitivity = randomValue();
        camerahorizontal.OnTrigger();

        cameravertical.RotationSensitivity = 10;
        camerahorizontal.RotationSensitivity = 10;
    }
}
