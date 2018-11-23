using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameCores : MonoBehaviour {

    void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }

    //void Start()
    //{
    //}
}
