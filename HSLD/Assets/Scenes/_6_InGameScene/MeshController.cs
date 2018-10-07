using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Terrain{
    DEFAULT,
    BARREN,
    MODERATION,
    COLD,
    SEA,
    MOUNTAIN
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
    // Use this for initialization
    public GameObject SceneManager;

    void Start () {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Barren");
        defaultMaterial = Resources.Load<Material>("M_Default");
        isAwake = false;
        isFixed = false;
        giveNumber++;
        MeshNumber = giveNumber;
        SceneManager = GameObject.Find("InGameSceneManager");
    }
	
	// Update is called once per frame
	void Update () {
        if (isAwake)
        {
            if (terrainstate == Terrain.DEFAULT) // 기본 터레인인거면 아무도 안건들였음을 의미 함.
            {
                if (GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount > 0)
                {
                    if (SceneManager.GetComponent<CardSystem>().pickedCard)
                    {
                        Debug.Log("picked");
                        GameObject picked = SceneManager.GetComponent<CardSystem>().pickedCard;

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
                        GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount--;
                    }
                   
                }
            }
            else // 그 외 지형카드로 색을 칠했었던거라면 다시 돌려줌
            {
                if (GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount < 10) {
                    GetComponent<MeshRenderer>().material = defaultMaterial;
                    terrainstate = Terrain.DEFAULT;

                    GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount++;
                }
            }

            isAwake = false;
        }

        // 턴이 종료됐어 이번에 점령이 확정 됐다면 isFixed 를 true로 바꿔줘

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