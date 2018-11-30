using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum MissionSet
{
    MAIN,
    SUB
}

public class MissionInfo : ScriptableObject
{
    public MissionSet missionSet;
    public int index;
    public string text;
    public int currentCnt;
    public int goalCnt;

    private void Awake()
    {
        currentCnt = 0;
    }
}
