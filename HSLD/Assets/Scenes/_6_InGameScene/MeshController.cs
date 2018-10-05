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
    public Terrain terrainstate;
    public bool isAwake;
    public bool isFixed;
    // Use this for initialization

    void Start () {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Barren");
        defaultMaterial = Resources.Load<Material>("M_Default");
        isAwake = false;
        isFixed = false;
    }
	
	// Update is called once per frame
	void Update () {
        if (isAwake)
        {
            if (terrainstate == Terrain.DEFAULT) // 기본 터레인인거면 아무도 안건들였음을 의미 함.
            {
                if (GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount > 0)
                {
                    //priorMaterial = GetComponent<MeshRenderer>().material; // 이전 머테리얼 저장해둠
                    GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
                    terrainstate = Terrain.BARREN;

                    GameObject.FindWithTag("MainCamera").GetComponent<CameraController>().ChangeableCount--;
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
        domMaterial = Resources.Load<Material>("M_Barren");
    }
}