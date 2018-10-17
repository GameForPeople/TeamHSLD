using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AllMeshController : MonoBehaviour {

    const int MaxMesh = 321;
    private static bool once;

    public static GameObject IngameManager;
    public static GameObject myPlanet;
    public int[] PickContainer;
    public GameObject[] AllContainer;
    // Use this for initialization
    void Start () {
        once = true;
        IngameManager = GameObject.Find("InGameSceneManager");
        myPlanet = GameObject.Find("Sphere_320Objects_40X");
        PickContainer = new int[12];
        AllContainer = new GameObject[MaxMesh];
    }

    // Update is called once per frame
    void Update()
    {
        if (once == true)
        {
            MakeAllContainer();
            once = false;
        }
    }

    public void MakeAllContainer()
    {
        for (int i = 1; i <= MaxMesh - 1; i++)
        {
            AllContainer[i] = GameObject.Find(i.ToString());
        }
    }

    public void Search()
    {
        for (int i = 1; i <= MaxMesh - 1; i++)
        {
            if (AllContainer[i].GetComponent<MeshController>().isFixed == true)
            {
                Debug.Log("Fixed : " + i);
                //고정 값 된 애들 검사
            }
            if (AllContainer[i].GetComponent<MeshController>().isMine == true) // 내 메시
            {
                Debug.Log("Mine : " + i);
                //고정 값 된 애들 검사
            }
        }
    }

    public bool isEmpty()
    {
        //int temp = 0;
        for(int i = 0; i< PickContainer.Length; i++)
        {
            if(PickContainer[i] != 0)
            {
                //temp++;
                return false;
            }
        }
        //if(temp == 1)
        //{
        //    Debug.Log("저장소는 비어있습니다!");
        //    return true;
        //}
        return true;
    }
}
