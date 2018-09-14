using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class UIText : MonoBehaviour {

    Text DebuggingText;
    public GameObject cameraObject;
    public Camera mainCamera;
    public GameObject planetObject;
    

    void Start()
    {
        DebuggingText = GetComponent<Text>();
        planetObject = GameObject.Find("Planet");
        cameraObject = GameObject.Find("Main Camera");
    } 

    void Update()
    {
        DebuggingText.text = "TouchCount : " + Input.touchCount + "\n";

        if (Input.touchCount >= 1)
        {
            for(int i = 0; i<Input.touchCount; i++)
            {
                if (Input.touchCount == 1)
                {
                    if (Input.GetTouch(0).phase == TouchPhase.Moved) {
                        DebuggingText.text +=
                            "[ Touch(0) Phase is Moved ] \n";
                    }
                    else
                    {
                        DebuggingText.text += "[ Touch(0) Another Phase ] \n";
                        RaycastHit hit;

                        Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

                        if (Physics.Raycast(ray, out hit))
                        {
                            DebuggingText.text +=
                                "[Pick Object] : " + hit.transform.name +
                                " // " + hit.transform.position + "\n";
                        }

                    }
                }

                DebuggingText.text +=
                    "Input.GetTouch(" + i + ") : " + Input.GetTouch(i).position.ToString() + "\n";

                if (Input.touchCount == 2)
                {
                    DebuggingText.text += "[Scale]" + "\n";
                }
            }
        }
    }
}