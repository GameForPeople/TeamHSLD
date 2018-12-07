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
    public Material priorMaterial;
    public Terrain priorState;

    public int MeshNumber;
    static int giveNumber;
    public Terrain terrainstate;
    public bool isAwake;
    public bool isFixed;
    public bool isMine;
    public bool isFlag;     //거점
    public int Linkednum;
    public GameObject[] JointMesh;
    public List<GameObject> NearMesh;
    // Use this for initialization

    private bool once;
    private Vector3 startPos;
    private Vector3 destinationPos;
    public bool isLandingSign;
    public GameObject TargetObject;

    const float initSize = 1.02f;
    const float landingSize = 1.05f;

    void Start () {
        if (isFlag == true)
        {
            terrainstate = Terrain.FLAG;
            gameObject.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_FlagAble");
        }
        else
        {
            terrainstate = Terrain.DEFAULT;
        }
        domMaterial = Resources.Load<Material>("M_Cold");
        priorMaterial = gameObject.GetComponent<Renderer>().material;
        priorState = gameObject.GetComponent<MeshController>().terrainstate;
        isAwake = false;
        isFixed = false;
        isMine = false;
        isLandingSign = false;
        once = false;
        giveNumber++;
        MeshNumber = giveNumber;
        name = giveNumber.ToString();
        Linkednum = 0;

        transform.position *= initSize; // 초기 테두리 사이즈
        startPos = transform.position;
        destinationPos = transform.position * landingSize; // Landing 사이즈
    }

    // Update is called once per frame
    void Update () {
        if (isLandingSign == true)
        {
            StartCoroutine(MoveUp());
            isLandingSign = false;
        }
        if (isFlag && once == false && AllMeshController.once)
        {
            for(int i = 1; i< AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer.Length - 1; i++)
            {
                if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]) // 에러 나와서 일단 넣음 
                {
                    float pTop = (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].transform.position - transform.position).magnitude;

                    if (pTop < 19.45f && pTop != 0)
                    {
                        NearMesh.Add(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]);
                        AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().priorMaterial = Resources.Load<Material>("M_JointFlag");

                        //Debug.Log(name);
                        //AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
                    }
                }
            } // Flagable이면 주변 매쉬 받아와


            once = true;
        }

        if (isAwake)
        {
            if (true) // isMoblie == true
            {
                CameraController.lastmesh = MeshNumber;
                Debug.Log("Pick : " + CameraController.lastmesh);
                int cameraYAmount = 138;
                Vector3 destinationPos = gameObject.transform.position.normalized * cameraYAmount;
                StartCoroutine(Camera.main.GetComponent<CameraController>().movePosition(Camera.main.transform.position, destinationPos));
            }
            
            if (terrainstate == Terrain.ABLE || terrainstate == Terrain.DEFAULT)
            {
                Picked();
            }
            else if (terrainstate == Terrain.FLAG)
            {
                //GetComponent<MeshController>().terrainstate = Terrain.FLAG;
                //GetComponent<Renderer>().material = Resources.Load<Material>("M_FlagAble");
                //EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[0], 1.03f);
                // 내 색 오브젝트 필요할듯?
            }
            else
            {
                GetComponent<MeshRenderer>().material = priorMaterial;
                terrainstate = priorState;
                for (int j = 0; j < 3; j++)
                {
                    if (JointMesh[j].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                    {
                        JointMesh[j].GetComponent<MeshController>().setDefault();
                    }
                }
                CameraController.ChangeableCount++;
            }

            // Able 다시 세팅
            AllMeshController.myPlanet.GetComponent<AllMeshController>().ResettingAble();

            isAwake = false;
        }

        // 턴이 종료됐어 이번에 점령이 확정 됐다면 isFixed 를 true로 바꿔줘

    }

    public IEnumerator MoveUp()
    {
        while (transform.position.magnitude <= destinationPos.magnitude - 0.6f)
        {
            transform.position = Vector3.Lerp(transform.position, destinationPos, Time.deltaTime/2.0f);

            yield return null;
        }
        // 객체 추가해서 달아주자.
        if(terrainstate == Terrain.MODERATION)
        {
            //20퍼센트 확률로 생기지 않음.
            if (randomValue(0, 100) < 80 || isFlag)
            {
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[randomValue(14, 18)], 1.03f);
            }
        } else if (terrainstate == Terrain.BARREN)
        {
            //70퍼센트 확률로 생기지 않음.
            if (randomValue(0, 100) < 30 || isFlag)
            {
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[randomValue(5, 13)], 1.03f);
            }

        } else if (terrainstate == Terrain.COLD)
        {
            //40퍼센트 확률로 생기지 않음.
            if (randomValue(0, 100) < 60 || isFlag)
            {
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[randomValue(1, 4)], 1.03f);
            }
        } else if (terrainstate == Terrain.SEA)
        {
            //90퍼센트 확률로 생기지 않음.
            if (randomValue(0, 100) < 10 || isFlag)
            {
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[randomValue(21, 22)], 1.03f);
            }
        } else if (terrainstate == Terrain.MOUNTAIN)
        {
            //10퍼센트 확률로 생기지 않음.
            if (randomValue(0, 100) < 90 || isFlag)
            {
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[randomValue(19, 20)], 1.03f);
            }
        }
    }

    public IEnumerator MoveDown()
    {
        while (transform.position.magnitude >= startPos.magnitude)
        {
            Debug.Log("찍히냐?");
            transform.position = Vector3.Lerp(transform.position, startPos * 1.0f, Time.deltaTime / 5);
            // 객체가 있었다면 객체들을 지워야 함.
            Destroy(TargetObject);
            yield return null;
        }
    }

    public void EulerRotCal(GameObject targetObj, GameObject buildingObj, float offset)
    {
        TargetObject = Instantiate(buildingObj);

        TargetObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        TargetObject.transform.parent = GameObject.Find("ObjectSet").transform;
        
        TargetObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);
        TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x + 180, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
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
    }
    //건조
    public void setBarren()
    {
        terrainstate = Terrain.BARREN;
        domMaterial = Resources.Load<Material>("M_Barren");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

    }
    //한랭
    public void setCold()
    {
        terrainstate = Terrain.COLD;
        domMaterial = Resources.Load<Material>("M_Cold");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라

    }

    public void setSea()
    {
        terrainstate = Terrain.SEA;
        domMaterial = Resources.Load<Material>("M_Sea");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라


    }

    public void setMountain()
    {
        terrainstate = Terrain.MOUNTAIN;
        domMaterial = Resources.Load<Material>("M_Mountain");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라


    }

    public void setFlag()
    {
        terrainstate = Terrain.FLAG;
        domMaterial = Resources.Load<Material>("M_FlagAble");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setDefault()
    {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Default");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }
}