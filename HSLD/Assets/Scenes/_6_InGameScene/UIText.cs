using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class UIText : MonoBehaviour {

    Text DebuggingText;
    public GameObject cameraObject;
    public Camera mainCamera;
    public GameObject planetObject;
    public Vector3 Vec3;

    private Vector4 ColorVec4;
    private GameObject PickedMesh;


    void Start()
    {
        DebuggingText = GetComponent<Text>();
        DebuggingText.color = new Color(1,1,1);
        //planetObject = GameObject.Find("Planet");
        //cameraObject = GameObject.Find("Main Camera");
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

                            PickedMesh = GameObject.Find(hit.transform.name);
                            MeshRenderer PickedRenderer = PickedMesh.GetComponent<MeshRenderer>();
                            ColorVec4 = PickedRenderer.material.color;

                            Vec3 = PickedRenderer.transform.position;
                            
                            DebuggingText.text +=
                                "[Pick Object] : " + hit.transform.name +
                                " // " + Vec3 + "\nHit Point : " + hit.point + "\nColor : " + ColorVec4 + "\n";

                            // hit.
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