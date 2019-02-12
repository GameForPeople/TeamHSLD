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

            SetMaterial(MeshData[MeshNumber]["meshState"].ToString()); // Material 세팅
            if(MeshData[MeshNumber]["meshObj"].ToString() != null) // Object 세팅
            {
                Debug.Log("Setting");
                SetObject(MeshData[MeshNumber]["meshState"].ToString());
            }
            // MoveObject 세팅
        }
        else
        {
            Debug.Log("non data");
        }
    }

    private void SetMaterial(string material)
    {
        if (material == "DEFAULT")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Default");
        }
        if (material == "MODERATION")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Moderation");
        }
        else if (material == "BARREN")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_BARREN");
        }
        else if (material == "COLD")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_COLD");
        }
        else if (material == "SEA")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Sea");
        }
        else if (material == "MOUNTAIN")
        {
            GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Mountain");
        }
    }

    private void SetObject(string material)
    {
        if (material == "DEFAULT")
        {
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.DEFAULT);
        }
        if (material == "MODERATION")
        {
            Debug.Log("Moderation");
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.MODERATION);
        }
        else if (material == "BARREN")
        {
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.BARREN);
        }
        else if (material == "COLD")
        {
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.COLD);
        }
        else if (material == "SEA")
        {
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.SEA);
        }
        else if (material == "MOUNTAIN")
        {
            gameObject.GetComponent<MeshController>().InstateTerrainObject(Terrain.MOUNTAIN);
        }
    }
    
}
