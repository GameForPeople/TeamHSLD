using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AllMeshController : MonoBehaviour {

    const int MaxMesh = 321;
    public static bool once;

    public static GameObject IngameManager;
    public static GameObject myPlanet;
    //public int[] PickContainer;
    public List<int> PickContainer;
    public List<GameObject> FlagContainer;
    public GameObject[] AllContainer;
    public GameObject[] buildingObj;
    static public AllMeshController instance_;
    static public int giveLinkNum = 0;

    // Use this for initialization
    void Start () {
        once = false;
        IngameManager = GameObject.Find("InGameSceneManager");
        myPlanet = GameObject.Find("Sphere_320Objects_40X");
        //PickContainer = new int[12];
        PickContainer = new List<int>();
        AllContainer = new GameObject[MaxMesh];
        if(instance_ == null)
            instance_ = this;
    }

    // Update is called once per frame
    void Update()
    {
        if (once == false)
        {
            MakeAllContainer();
            
            once = true;
        }
    }

    public void MakeAllContainer()
    {
        for (int i = 1; i < MaxMesh; i++)
        {
            AllContainer[i] = GameObject.Find(i.ToString());
            if (AllContainer[i].GetComponent<MeshController>().isFlagable) // 해당 메시가 Flagable이라면?
            {
                FlagContainer.Add(AllContainer[i]);
                AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_FlagAble");
                AllContainer[i].GetComponent<MeshController>().EulerRotCal(AllContainer[i], buildingObj[0], 1.03f);
            }
        }
    }

    public void SearchALL()
    {
        for (int i = 1; i <= MaxMesh - 1; i++)
        {
            if (AllContainer[i].GetComponent<MeshController>().isFixed == true)
            {
                Debug.Log("Fixed : " + i);
                //고정 값 된 애들 검사
            }
            if (AllContainer[i].GetComponent<MeshController>().isMine == true) // 내 메시
            {
                Debug.Log("Mine : " + i);
                //고정 값 된 애들 검사
            }
        }
    }

    public bool isEmpty()
    {
        for(int i = 0; i< PickContainer.Count; i++)
        {
            if(PickContainer[i] != 0)
            {
                return false;
            }
        }
        return true;
    }

    public void ArrangeLinkedMesh(string meshnum, Terrain type)
    {
        GameObject CheckMesh = GameObject.Find(meshnum.ToString());
        int keepLinknum = 0;

        for(int i = 0; i < 3; i++)
        {
            // 타입이 같은 JointMesh를 만났어!
            if (CheckMesh.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().terrainstate == type)
            {
                if (CheckMesh.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().Linkednum !=
                    CheckMesh.GetComponent<MeshController>().Linkednum) // 컨테이너에 들어있는 애가 아닐 때만 
                {
                    keepLinknum = CheckMesh.GetComponent<MeshController>().Linkednum;
                    int ChangingLinkedNum = CheckMesh.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().Linkednum;
                    for (int j = 1; j < AllContainer.Length; j++)
                    {
                        if (AllContainer[j].GetComponent<MeshController>().Linkednum == ChangingLinkedNum)
                        {
                            //Debug.Log(AllContainer[j].GetComponent<MeshController>().Linkednum + "[만나쓰]-->" + keepLinknum);
                            AllContainer[j].GetComponent<MeshController>().Linkednum = keepLinknum;
                        }
                    }
                }
            }
        }
    }

    public void InitLinkedMesh(int Linknumber) // 해당
    {
        for(int i = 1; i< AllContainer.Length; i++)
        {
            if(AllContainer[i].GetComponent<MeshController>().Linkednum == Linknumber)
            {
                StartCoroutine(AllContainer[i].GetComponent<MeshController>().MoveDown());
                AllContainer[i].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                AllContainer[i].GetComponent<MeshController>().GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                AllContainer[i].GetComponent<MeshController>().Linkednum = 0;
                AllContainer[i].GetComponent<MeshController>().isFixed = false;
                AllContainer[i].GetComponent<MeshController>().isMine = false;
            }
        }
    }
}