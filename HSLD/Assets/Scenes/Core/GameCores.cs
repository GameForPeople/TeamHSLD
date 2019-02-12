using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameCores : MonoBehaviour {

    void Awake()
    {
        //YSH 추가합니다.
        Screen.sleepTimeout = SleepTimeout.NeverSleep;
        Screen.SetResolution(1280, 720, true);

        DontDestroyOnLoad(gameObject);
    }

    //void Start()
    //{
    //}
}   
