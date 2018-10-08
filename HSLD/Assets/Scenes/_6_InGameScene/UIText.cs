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

    private int TestCount;
    private int MeshNum;

    void Start()
    {
        DebuggingText = GetComponent<Text>();
        DebuggingText.color = new Color(1,1,1);
        //planetObject = GameObject.Find("Planet");
        cameraObject = GameObject.Find("Main Camera");
    } 

    void Update()
    {
        TestCount = CameraController.ChangeableCount;

        DebuggingText.text = "TouchCount : " + Input.touchCount + " Count : " + TestCount + "\n";

        if (Input.touchCount >= 1)
        {
            for(int i = 0; i<Input.touchCount; i++)
            {
                if (Input.touchCount == 1)
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
                        if (PickedMesh)
                        {
                            MeshNum = PickedMesh.GetComponent<MeshController>().MeshNumber;
                            DebuggingText.text +=
                                "[Pick Object] : " + hit.transform.name +
                                "\nMeshNum : " + MeshNum +
                                "\nisAwake : " + PickedMesh.GetComponent<MeshController>().isAwake +
                                "\nTerrainState : " + PickedMesh.GetComponent<MeshController>().terrainstate.ToString() + "\n";
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