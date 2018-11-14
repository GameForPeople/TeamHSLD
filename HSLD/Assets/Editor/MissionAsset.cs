using UnityEngine;
using UnityEditor;

public class MissionAsset : ScriptableObject
{
    [MenuItem("Assets/Create/Mission")]
    public static void CreateAsset()
    {
        ScriptableObjectUtility.CreateAsset<MissionInfo>();
    }
}