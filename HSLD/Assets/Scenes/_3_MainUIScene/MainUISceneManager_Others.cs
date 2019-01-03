﻿using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{ 
    // Use this for initialization
    void StartForOthers ()
    {
        GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(true);
        GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas_Dynamic").transform.Find("Money_Text").gameObject.GetComponent<Text>().text
            = networkObject.money.ToString();

        GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas").transform.Find("NickName_Text").gameObject.GetComponent<Text>().text
            = networkObject.nickName.ToString();
    }

}
