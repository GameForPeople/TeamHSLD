using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using LitJson;

public class LoadJsonData : MonoBehaviour {
    public List<MeshInfo> settingMesh = new List<MeshInfo>();
    
    void Start () {
        LoadMeshData();
    }
	
	// Update is called once per frame
	void Update () {
		
	}
    
    public void LoadMeshData()
    {
        Debug.Log("LoadJsonData");

        if(File.Exists(Application.dataPath+ "/Resources/Data/MeshInfoData.json"))
        {
            string jsonStr = File.ReadAllText(Application.dataPath + "/Resources/Data/MeshInfoData.json");

            Debug.Log(jsonStr);
            JsonData MeshData = JsonMapper.ToObject(jsonStr);

            for(int i = 1; i < 321; i++)
            {

            }
        }
        else
        {
            Debug.Log("non data");
        }
    }
}
