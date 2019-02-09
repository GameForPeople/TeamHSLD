using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using LitJson;

public class LoadJsonData : MonoBehaviour {
    public List<MeshInfo> settingMesh = new List<MeshInfo>();
    public int MeshNumber;          // Mesh번호 매기기 (1~320)
    static int giveFirstNumber;          // Mesh번호 매기기를 위한 static값


    void Start () {
        MeshNumber = giveFirstNumber;
        name = giveFirstNumber.ToString();
        giveFirstNumber++;

        LoadMeshData();
    }
	
	// Update is called once per frame
	void Update () {
		
	}
    
    public void LoadMeshData()
    {
        Debug.Log("LoadJsonData");

        if (File.Exists(Application.dataPath + "/Resources/Data/MeshInfoData.json"))
        {
            string jsonStr = File.ReadAllText(Application.dataPath + "/Resources/Data/MeshInfoData.json");

            //Debug.Log(jsonStr);
            JsonData MeshData = JsonMapper.ToObject(jsonStr);

            Debug.Log(MeshData[MeshNumber]["meshID"].ToString() + "," + MeshData[MeshNumber]["meshState"].ToString());

            if (MeshData[MeshNumber]["meshState"].ToString() == "MODERATION")
            {
                GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Moderation");
            }
        }
        else
        {
            Debug.Log("non data");
        }
    }
    
}
