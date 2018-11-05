using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public enum Terrain{
    DEFAULT,
    MODERATION,
    BARREN,
    COLD,
    SEA,
    MOUNTAIN,
    ABLE,
    FLAG
}

public class MeshController : MonoBehaviour {
    private int dominator;
    private Material domMaterial;
    private Material defaultMaterial;
    private Material priorMaterial;
    public int MeshNumber;
    static int giveNumber;
    public Terrain terrainstate;
    public bool isAwake;
    public bool isFixed;
    public bool isMine;
    public bool isFlagable;     //거점
    public int Linkednum;
    public GameObject[] JointMesh;
    public List<GameObject> DomMesh;
    // Use this for initialization

    private bool once;
    private Vector3 startPos;
    private Vector3 destinationPos;
    public bool isLandingSign;

    void Start () {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Cold");
        defaultMaterial = Resources.Load<Material>("M_Default");
        isAwake = false;
        isFixed = false;
        isMine = false;
        isLandingSign = false;
        giveNumber++;
        MeshNumber = giveNumber;
        name = giveNumber.ToString();
        //JointMesh = new GameObject[3];
        once = false;
        Linkednum = 0;
        startPos = transform.position;
        destinationPos = transform.position * 1.05f;
    }
	
	// Update is called once per frame
	void Update () {
        if (isLandingSign == true)
        {
            StartCoroutine(MoveUp());
            isLandingSign = false;
        }
        if (isFlagable && once == false && AllMeshController.once)
        {
            for(int i = 1; i< AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer.Length - 1; i++)
            {
                if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]) // 에러 나와서 일단 넣음 
                {
                    float pTop = (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].transform.position - transform.position).magnitude;

                    if (pTop < 19 && pTop != 0)
                    {
                        DomMesh.Add(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]);

                        //Debug.Log(name);
                        //AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                    }
                }
            } // Flagable이면 주변 매쉬 받아와


            once = true;
        }

        if (isAwake)
        {
            if (terrainstate == Terrain.ABLE)
            {
                Picked();
            }
            else if (terrainstate == Terrain.DEFAULT) {
                Picked();
            }
            else
            {
                GetComponent<MeshRenderer>().material = defaultMaterial;
                terrainstate = Terrain.DEFAULT;
                for (int j = 0; j < 3; j++)
                {
                    if (JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                    {
                        JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                        JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                    }
                }
                CameraController.ChangeableCount++;
            }

            // Able 다시 세팅
            for (int i = 0; i < AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer.Count; i++)
            {
                GameObject FindObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[i].ToString());

                // 우선 able 다 지워
                for (int j = 0; j < 3; j++)
                {
                    if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                    {
                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.DEFAULT;
                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Default");
                    }
                }

                // 마지막 회전할 때 우선순위에 따라 세팅
                if (i == AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer.Count - 1)
                {
                    GameObject FirstObject = GameObject.Find(AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer[0].ToString());

                    // 1.거점 주변 세팅
                    if (PCverPIcking.isCheck == false)
                    {
                        int DomCount = 0;
                        for (int k = 0; k < AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count; k++)
                        {
                            if (AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT ||
                                AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k].GetComponent<MeshController>().terrainstate == Terrain.ABLE)
                            {
                                AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                            }
                            else
                            {
                                DomCount++;
                            }
                        }
                        if (DomCount == AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count) // 거점 획득
                        {
                            PCverPIcking.isGetFlag = true;
                            GameObject.FindWithTag("GameManager").GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
                        }
                        else
                        {
                            PCverPIcking.isGetFlag = false;
                        }
                    }

                    // 2.Able에 대한 세팅해줘
                    for (int j = 0; j < 3; j++)
                    {
                        if (PCverPIcking.isGetFlag == false) // 거점 획득 전
                        {
                            if (FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT) // able일 때 default로 바꿔줘
                            {
                                for (int k = 0; k < AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count; k++)
                                {
                                    if (AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k] ==
                                        FindObject.GetComponent<MeshController>().JointMesh[j])
                                    {
                                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate = Terrain.ABLE;
                                        FindObject.GetComponent<MeshController>().JointMesh[j].GetComponent<Renderer>().material = Resources.Load<Material>("M_Able");
                                    }
                                }
                            }

                            if (FirstObject.GetComponent<MeshController>().JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.DEFAULT)
                            {
                                for (int k = 0; k < AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh.Count; k++)
                                {
                                    if (AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[0].GetComponent<MeshController>().DomMesh[k] ==
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
                    for (int j = 0; j < AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer.Count; j++)
                    {
                        AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<MeshController>().terrainstate = Terrain.FLAG;
                        AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer[j].GetComponent<Renderer>().material
                            = Resources.Load<Material>("M_FlagAble");
                    }
                }
            }

            isAwake = false;
        }

        // 턴이 종료됐어 이번에 점령이 확정 됐다면 isFixed 를 true로 바꿔줘

    }

    public IEnumerator MoveUp()
    {
        while (transform.position.magnitude <= destinationPos.magnitude - 0.6)
        {
            transform.position = Vector3.Lerp(transform.position, destinationPos*1.0f, Time.deltaTime / 5);

            yield return null;
        }
    }

    public void Picked()
    {
        if (AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard)
        {
            Debug.Log("picked");
            GameObject picked = AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard;

            CameraController.ChangeableCount--;

           
            if (picked.name.Equals("TerrainCardImg1"))
            {
                Debug.Log("setModeration");
                setModeration();
            }
            else  if (picked.name.Equals("TerrainCardImg2"))
            {
                Debug.Log("setBarren");
                setBarren();
            }
            else if (picked.name.Equals("TerrainCardImg3"))
            {
                Debug.Log("TerrainCardImg3");
                setCold();
            }
            else if (picked.name.Equals("TerrainCardImg4"))
            {
                Debug.Log("setSea");
                setSea();
            }
            else if (picked.name.Equals("TerrainCardImg5"))
            {
                Debug.Log("setMountain");
                setMountain();
            }
            else if (picked.name.Equals("EventCardImg1"))
            {
                Debug.Log("임시");
                setCold();
            }
            else if (picked.name.Equals("EventCardImg2"))
            {
                Debug.Log("임시");
                setCold();
            }
        }
    }

    private int randomValue(int min, int max)
    {
        return Random.Range(min, max + 1);
    }

    //적당
    public void setModeration()
    {
        terrainstate = Terrain.MODERATION;
        domMaterial = Resources.Load<Material>("M_Moderation");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

        //20퍼센트 확률로 생기지 않음.
        if (randomValue(0, 100) < 20 || isFlagable)
            return;
        GameObject.FindWithTag("GameManager").GetComponent<BuildOnPlanet>().EulerRotCal(gameObject, AllMeshController.instance_.buildingObj[randomValue(14, 18)], 0.5f);
    }
    //건조
    public void setBarren()
    {
        terrainstate = Terrain.BARREN;
        domMaterial = Resources.Load<Material>("M_Barren");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

        //70퍼센트 확률로 생기지 않음.
        if (randomValue(0, 100) < 70 || isFlagable)
            return;
        GameObject.FindWithTag("GameManager").GetComponent<BuildOnPlanet>().EulerRotCal(gameObject, AllMeshController.instance_.buildingObj[randomValue(5, 13)], 0.5f);
    }
    //한랭
    public void setCold()
    {
        terrainstate = Terrain.COLD;
        domMaterial = Resources.Load<Material>("M_Cold");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

        //40퍼센트 확률로 생기지 않음.
        if (randomValue(0, 100) < 40 || isFlagable)
            return;
        GameObject.FindWithTag("GameManager").GetComponent<BuildOnPlanet>().EulerRotCal(gameObject, AllMeshController.instance_.buildingObj[randomValue(1,4)], 0.5f);
    }

    public void setSea()
    {
        terrainstate = Terrain.SEA;
        domMaterial = Resources.Load<Material>("M_Sea");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

        //90퍼센트 확률로 생기지 않음.
        if (randomValue(0, 100) < 90 || isFlagable)
            return;
        GameObject.FindWithTag("GameManager").GetComponent<BuildOnPlanet>().EulerRotCal(gameObject, AllMeshController.instance_.buildingObj[randomValue(21, 22)], 0.5f);
    }

    public void setMountain()
    {
        terrainstate = Terrain.MOUNTAIN;
        domMaterial = Resources.Load<Material>("M_Mountain");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

        //10퍼센트 확률로 생기지 않음.
        if (randomValue(0, 100) < 10 || isFlagable)
            return;
        GameObject.FindWithTag("GameManager").GetComponent<BuildOnPlanet>().EulerRotCal(gameObject, AllMeshController.instance_.buildingObj[randomValue(19, 20)], 0.5f);
    }
}