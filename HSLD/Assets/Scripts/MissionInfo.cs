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
}
