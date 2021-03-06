﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class UIText : MonoBehaviour {

    Text DebuggingText;
    public GameObject cameraObject;
    public Camera mainCamera;
    public GameObject planetObject;
    public Vector3 Vec3;
    
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
        //DebuggingText.text = "TouchCount : " + Input.touchCount + " Count : " + CameraController.ChangeableCount + "\n";
        
        if(GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>() != null)
        {
            if (!GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
                return;

            if (CameraController.ChangeableCount < 1)
            {
                GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>().diceCntingUI.transform.GetComponentInChildren<Text>().text = "";
                GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>().setTerrainCanvas.SetActive(true);
            }

            else
            {
                GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>().diceCntingUI.transform.GetComponentInChildren<Text>().text = CameraController.DiceCount.ToString();
                GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>().setTerrainCanvas.SetActive(false);
            }
        }
        else
        {
            if (!GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
                return;

            if (CameraController.ChangeableCount < 1)
            {
                GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().diceCntingUI.transform.GetComponentInChildren<Text>().text = "";
                GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().setTerrainCanvas.SetActive(true);
            }

            else
            {
                GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().diceCntingUI.transform.GetComponentInChildren<Text>().text = CameraController.DiceCount.ToString();
                GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().setTerrainCanvas.SetActive(false);
            }
        }
        
        //if (Input.touchCount >= 1)
        //{
        //    for(int i = 0; i<Input.touchCount; i++)
        //    {
        //        if (Input.touchCount == 1)
        //        {
        //            DebuggingText.text += "[ Touch(0) Another Phase ] \n";
        //            RaycastHit hit;

        //            Ray ray = mainCamera.ScreenPointToRay(Input.GetTouch(0).position);

        //            if (Physics.Raycast(ray, out hit))
        //            {

        //                PickedMesh = GameObject.Find(hit.transform.name);
        //                MeshRenderer PickedRenderer = PickedMesh.GetComponent<MeshRenderer>();

        //                Vec3 = PickedRenderer.transform.position;
        //                if (PickedMesh)
        //                {
        //                    MeshNum = PickedMesh.GetComponent<MeshController>().MeshNumber;
        //                    DebuggingText.text +=
        //                        "[Pick Object] : " + hit.transform.name +
        //                        "\nMeshNum : " + MeshNum +
        //                        "\nisAwake : " + PickedMesh.GetComponent<MeshController>().isAwake +
        //                        "\nTerrainState : " + PickedMesh.GetComponent<MeshController>().terrainstate.ToString() + "\n";
        //                }
        //            }
        //        }

        //        DebuggingText.text +=
        //            "Input.GetTouch(" + i + ") : " + Input.GetTouch(i).position.ToString() + "\n";

        //        if (Input.touchCount == 2)
        //        {
        //            DebuggingText.text += "[Scale]" + "\n";
        //        }
        //    }
        //}
    }
}