using UnityEngine;
using UnityEditor;

public class CardAsset : ScriptableObject
{
    [MenuItem("Assets/Create/Card")]
    public static void CreateAsset()
    {
        ScriptableObjectUtility.CreateAsset<CardInfo>();
    }
}