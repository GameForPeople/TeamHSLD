using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameCores : MonoBehaviour {
    GameObject networkManagerObject;
    NetworkManager networkManager;

    void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }

    void Start()
    {
        networkManagerObject = transform.Find("NetworkManager").gameObject;
        networkManagerObject.AddComponent<NetworkManager>();
        networkManager = this.transform.Find("NetworkManager").GetComponent<NetworkManager>();
    }

    public void StartNetworkFunction()
    {
        networkManager.StartNetworkFunction();
    }
}
