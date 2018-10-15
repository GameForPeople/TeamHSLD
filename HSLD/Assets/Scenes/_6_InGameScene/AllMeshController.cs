using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AllMeshController : MonoBehaviour {

    const int MaxMesh = 320;
    bool once;

    public static GameObject IngameManager;

    public static int[] PickContainer;
    public static GameObject[] AllContainer;
    // Use this for initialization
    void Start () {
        IngameManager = GameObject.Find("InGameSceneManager");
        PickContainer = new int[12];
        AllContainer = new GameObject[320];
        once = true;
    }
	
	// Update is called once per frame
	void Update () {
		if (once == true)
        {
            MakeAllContainer();
            once = false;
        }
    }


    public void MakeAllContainer()
    {
        for (int i = 1; i <= MaxMesh; i++)
        {
            GameObject Check = GameObject.Find(i.ToString());
            if (Check.GetComponent<MeshController>().isFixed == true)
            {
                AllContainer[i] = Check;
            }
        }
    }

    public static void Search()
    {
        for (int i = 0; i < MaxMesh; i++)
        {
            if (AllContainer[i].GetComponent<MeshController>().isFixed == true)
            {

                Debug.Log("Fixed : " + i);
                //고정 값 된 애들 검사
            }
            if (AllContainer[i].GetComponent<MeshController>().isMine == true)
            {

                Debug.Log("Mine : " + i);
                //고정 값 된 애들 검사
            }
        }
    }
}
