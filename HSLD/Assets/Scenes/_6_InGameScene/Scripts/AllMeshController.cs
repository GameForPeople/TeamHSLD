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
    public GameObject myFlag;
    public GameObject[] AllContainer;
    public GameObject[] buildingObj;
    public GameObject[] MovingObj;
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
            if (AllContainer[i].GetComponent<MeshController>().isFlag) // 해당 메시가 Flagable이라면?
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

    public void CleanPickContainer()
    {
        if (!isEmpty())
        {
            Camera.main.GetComponent<CameraShake>().ShakeOnce();
        }
        int Length = PickContainer.Count;
        bool temp = false;
        for (int i = 0; i < Length; i++)
        {
            GameObject target = GameObject.Find(PickContainer[i].ToString());
            Debug.Log("delete : " + target.name);
            Camera.main.GetComponent<PCverPIcking>().DeleteAble(target);
            target.GetComponent<Renderer>().material = target.GetComponent<MeshController>().priorMaterial;
            target.GetComponent<MeshController>().terrainstate = target.GetComponent<MeshController>().priorState;
            if (target == myFlag)
            {
                temp = true;
            }
        }
        AllPriorSetting();
        if (temp)
        {
            NearMeshSetting();
            myFlag = null;
            CameraController.ChangeableCount--;
            temp = false;
        }
        PickContainer.Clear();
        CameraController.ChangeableCount += Length; 

    }

    public void NearMeshSetting()
    {
        for(int i = 0; i< myFlag.GetComponent<MeshController>().NearMesh.Count; i++)
        {
            myFlag.GetComponent<MeshController>().NearMesh[i].GetComponent<MeshController>().setDefault();
        }
    }

    public void AllPriorSetting()
    {
        for (int i = 1; i < MaxMesh; i++)
        {
            AllContainer[i].GetComponent<MeshController>().priorMaterial = AllContainer[i].GetComponent<MeshRenderer>().material;
            AllContainer[i].GetComponent<MeshController>().priorState = AllContainer[i].GetComponent<MeshController>().priorState;
        }
    }

    public void ResettingAble()
    {
        for (int i = 0; i < myPlanet.GetComponent<AllMeshController>().PickContainer.Count; i++)
        {
            GameObject FindObject = GameObject.Find(myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());

            // 우선 able 다 지워
            for (int j = 0; j < 3; j++)
            {
                if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                {
                    FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().setDefault();
                }
            }

            // 마지막 회전할 때 우선순위에 따라 세팅
            if (i == myPlanet.GetComponent<AllMeshController>().PickContainer.Count - 1)
            {
                GameObject FirstObject = GameObject.Find(myPlanet.GetComponent<AllMeshController>().PickContainer[0].ToString());

                // 1.거점 주변 세팅
                if (PCverPIcking.isDominatedConfirm == false)
                {
                    int DomCount = 0;
                    for (int k = 0; k < myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; k++)
                    {
                        if (myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT ||
                            myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k].GetComponent<MeshController>().terrainstate == Terrain.ABLE)
                        {
                            myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                        }
                        else
                        {
                            DomCount++;
                        }
                    }
                    if (DomCount == myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count) // 거점 획득
                    {
                        PCverPIcking.isDominatedCheck = true;
                        GameObject.FindWithTag("GameManager").GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
                    }
                    else
                    {
                        PCverPIcking.isDominatedCheck = false;
                    }
                }

                // 2.Able에 대한 세팅해줘
                for (int j = 0; j < 3; j++)
                {
                    if (PCverPIcking.isDominatedCheck == false) // 거점 획득 전
                    {
                        if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // able일 때 default로 바꿔줘
                        {
                            for (int k = 0; k < myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; k++)
                            {
                                if (myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k] ==
                                    FindObject.GetComponent<MeshController>().JointMesh[j])
                                {
                                    FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                                    FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                                }
                            }
                        }

                        if (FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT)
                        {
                            for (int k = 0; k < myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; k++)
                            {
                                if (myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[k] ==
                                    FirstObject.GetComponent<MeshController>().JointMesh[j])
                                {
                                    FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                                    FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                                }
                            }
                        }
                    }
                    else // 거점 획득 후
                    {
                        if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // able일 때 default로 바꿔줘
                        {
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                            FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                        }
                        if (FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT)
                        {
                            FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                            FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                        }
                    }
                }

                // 3.마지막으로 거점 세팅
                for (int j = 0; j < myPlanet.GetComponent<AllMeshController>().FlagContainer.Count; j++)
                {
                    myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().terrainstate = Terrain.FLAG;
                    myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_FlagAble");
                }
            }
        }
    }
}
