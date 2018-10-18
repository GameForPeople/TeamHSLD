﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public enum Terrain{
    DEFAULT,
    BARREN,
    MODERATION,
    COLD,
    SEA,
    MOUNTAIN,
    ABLE
}

public class MeshController : MonoBehaviour {
    private int dominator;
    private Material domMaterial;
    private Material defaultMaterial;
    private Material priorMaterial;
    public int MeshNumber;
    static int giveNumber;
    public Terrain terrainstate;
    public bool isAwake;
    public bool isFixed;
    public bool isMine;
    public GameObject[] JointMesh;
    public GameObject[] Point_1;
    public GameObject[] Point_2;
    public GameObject[] Point_3;
    
    // Use this for initialization

    void Start () {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Cold");
        defaultMaterial = Resources.Load<Material>("M_Default");
        isAwake = false;
        isFixed = false;
        isMine = false;
        giveNumber++;
        MeshNumber = giveNumber;
        name = giveNumber.ToString();

        //JointMesh = new GameObject[3];
        Point_1 = new GameObject[6];
        Point_2 = new GameObject[6];
        Point_3 = new GameObject[6];
    }
	
	// Update is called once per frame
	void Update () {
        if (isAwake)
        {
            if (terrainstate == Terrain.ABLE)
            {
                Picked();
            }
            else if (terrainstate == Terrain.DEFAULT) {
                Picked();
            }
            else
            {
                GetComponent<MeshRenderer>().material = defaultMaterial;
                terrainstate = Terrain.DEFAULT;
                for (int j = 0; j < 3; j++)
                {
                    if (JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                    {
                        JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                        JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                    }
                }
                CameraController.ChangeableCount++;
            }

            for (int i = 0; i < AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer.Length; i++) // Able 다시 세팅
            {
                for (int j = 0; j < 3; j++)
                {
                    if (AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i] != 0) // 비어있지 않으면 주변애들을 M_Able로 바꿔줘
                    {
                        GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());
                        if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // able일 때 default로 바꿔줘
                        {
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                        }
                    }
                }
            }

            isAwake = false;
        }

        // 턴이 종료됐어 이번에 점령이 확정 됐다면 isFixed 를 true로 바꿔줘

    }

    public void Picked()
    {
        if (AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard)
        {
            Debug.Log("picked");
            GameObject picked = AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard;

            CameraController.ChangeableCount--;

            if (picked.name.Equals("TerrainCardImg1"))
            {
                Debug.Log("TerrainCardImg1");
                setBarren();
            }
            else if (picked.name.Equals("TerrainCardImg2"))
            {
                Debug.Log("TerrainCardImg2");
                setModeration();
            }
            else if (picked.name.Equals("TerrainCardImg3"))
            {
                Debug.Log("TerrainCardImg3");
                setCold();
            }
            else if (picked.name.Equals("TerrainCardImg4"))
            {
                Debug.Log("임시");
                setCold();
            }
            else if (picked.name.Equals("TerrainCardImg5"))
            {
                Debug.Log("임시");
                setCold();
            }
            else if (picked.name.Equals("EventCardImg1"))
            {
                Debug.Log("EventCardImg1");
                setSea();
            }
            else if (picked.name.Equals("EventCardImg2"))
            {
                Debug.Log("EventCardImg2");
                setMountain();
            }
        }
    }

    public void setBarren()
    {
        terrainstate = Terrain.BARREN;
        domMaterial = Resources.Load<Material>("M_Barren");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setModeration()
    {
        terrainstate = Terrain.MODERATION;
        domMaterial = Resources.Load<Material>("M_Moderation");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setCold()
    {
        terrainstate = Terrain.COLD;
        domMaterial = Resources.Load<Material>("M_Cold");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setSea()
    {
        terrainstate = Terrain.SEA;
        domMaterial = Resources.Load<Material>("M_Sea");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setMountain()
    {
        terrainstate = Terrain.MOUNTAIN;
        domMaterial = Resources.Load<Material>("M_Mountain");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }
}