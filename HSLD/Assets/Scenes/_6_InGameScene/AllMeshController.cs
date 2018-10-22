﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AllMeshController : MonoBehaviour {

    const int MaxMesh = 321;
    public static bool once;

    public static GameObject IngameManager;
    public static GameObject myPlanet;
    //public int[] PickContainer;
    public List<int> PickContainer;
    public List<GameObject> FlagContainer;
    public GameObject[] AllContainer;
    // Use this for initialization
    void Start () {
        once = false;
        IngameManager = GameObject.Find("InGameSceneManager");
        myPlanet = GameObject.Find("Sphere_320Objects_40X");
        //PickContainer = new int[12];
        PickContainer = new List<int>();
        AllContainer = new GameObject[MaxMesh];
    }

    // Update is called once per frame
    void Update()
    {
        if (once == false)
        {
            MakeAllContainer();
            
            once = true;
        }
    }

    public void MakeAllContainer()
    {
        for (int i = 1; i < MaxMesh; i++)
        {
            AllContainer[i] = GameObject.Find(i.ToString());
            if (AllContainer[i].GetComponent<MeshController>().isFlagable) // 해당 메시가 Flagable이라면?
            {
                FlagContainer.Add(AllContainer[i]);
                AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_FlagAble");
            }
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
        for(int i = 0; i< PickContainer.Count; i++)
        {
            if(PickContainer[i] != 0)
            {
                return false;
            }
        }
        return true;
    }

    public int DeleteCheckStart()
    {
        for(int i = 0; i < PickContainer.Count; i++)
        {
            if (PickContainer[i] != 0)
            {
                return PickContainer[i];
            } // 가장 먼저 0이 안들어가는 부분
        }
        return -1;
    }

    public int DeleteCheckLast()
    {
        for (int i = 0; i < PickContainer.Count; i++)
        {
            if (PickContainer[i] != 0)
            {
                return PickContainer[i];
            } // 가장 먼저 0이 안들어가는 부분
        }
        return -1;
    }
}
