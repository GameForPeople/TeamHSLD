using UnityEngine;
using UnityEditor;

public class LoadingAsset : ScriptableObject
{
    [MenuItem("Assets/Create/Load")]
    public static void CreateAsset()
    {
        ScriptableObjectUtility.CreateAsset<LoadingInfo>();
    }
}