using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

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
    public bool isMine;
    public GameObject[] Link;

    private int[] iLink;
    private float[] fLink;
    
    private bool once;


    // Use this for initialization

    void Start () {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Cold");
        defaultMaterial = Resources.Load<Material>("M_Default");
        isAwake = false;
        isFixed = false;
        isMine = false;
        once = true;
        giveNumber++;
        MeshNumber = giveNumber;
        name = giveNumber.ToString();

        Link = new GameObject[3];
        iLink = new int[3];
        fLink = new float[3];
    }
	
	// Update is called once per frame
	void Update () {
        if (isAwake)
        {
            if (once == true)
            {
                float min = 999;
                for (int i = 1; i < AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer.Length; i++)
                {
                    if (i == MeshNumber) continue;

                    Vector3 otherMeshPosition = AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].transform.position;
                    Vector3 pointTopoint = transform.position - otherMeshPosition; // 내 포지션과 다른 포지션을 빼
                    float distance = pointTopoint.magnitude;

                    if (distance < 12.6)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (Link[j] == null)
                            {
                                Link[j] = GameObject.Find(i.ToString());
                                Debug.Log(distance);
                                break;
                            }
                        }
                    }
                }


                once = false;
            }

            if (terrainstate == Terrain.DEFAULT) // 기본 터레인인거면 아무도 안건들였음을 의미 함.
            {
                // 임시

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
            else // 그 외 지형카드로 색을 칠했었던거라면 다시 돌려줌
            {
                GetComponent<MeshRenderer>().material = defaultMaterial;
                terrainstate = Terrain.DEFAULT;
                CameraController.ChangeableCount++;
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