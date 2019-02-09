using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using LitJson;

public class MeshInfo
{
    public string meshID;
    public string meshState;

    public MeshInfo(string _meshID, string _meshState)
    {
        meshID = _meshID;
        meshState = _meshState;
    }
}

public class SaveJsonData : MonoBehaviour {

    public List<MeshInfo> meshInfoList = new List<MeshInfo>();
    
    public void SaveMeshData()
    {
        Debug.Log("SaveJsonData");

        for(int i = 1; i < 321; i++)
        {
            Debug.Log(i);
        
            meshInfoList.Add(new MeshInfo(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().name
                , AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().terrainstate.ToString()));
        
            Debug.Log(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().name);
            Debug.Log(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().terrainstate.ToString());
        }

        JsonData infoJson = JsonMapper.ToJson(meshInfoList);

        File.WriteAllText(Application.dataPath + "/Resources/Data/MeshInfoData.json", infoJson.ToString());
    }
}
