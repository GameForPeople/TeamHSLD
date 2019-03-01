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
    FLAG,
    JOINT
}

public enum Identify
{
    NEUTRALITY,     // 중립
    ALLY,           // 아군
    ENEMY           // 적군
}

public class MeshController : MonoBehaviour {
    private int dominator;
    private Material domMaterial;   // Mesh의 Material
    public Material priorMaterial;  // Mesh가 이전에 갖고 있는 Material정보
    public Terrain priorState;      // Mesh가 이전에 갖고 있던 State정보
    public Identify currentIdentify = Identify.NEUTRALITY;

    public int MeshNumber;          // Mesh번호 매기기 (1~320)
    public int LinkedNumber;        // Linkednum이 같다면 같은 state로 붙어있는 Mesh들

    public GameObject terrainObj;  // Terrain이 갖는 오브젝트
    public Terrain terrainstate;    // Mesh의 터레인 상태
    public bool isAwake;            // Click 된 후 알고리즘 작동
    public bool isFixed;            // 턴이 종료되어 해당 Mesh의 변화가 확정됨
    public bool isMine;             // 턴이 종료되어 내 Mesh인 것이 확정됨
    public bool isFlag;             // 거점
    public bool isFlagMesh = false;

    public GameObject[] JointMesh = new GameObject[3];  // 근처 3가지 Mesh
    public List<GameObject> NearMesh;                   // 근처 9가지 Mesh
    public bool isCheck = false;
    // Use this for initialization

    private bool temp;              // Mesh관리 할 때 임의로 사용한 변수
    private Vector3 startPos;       // Camera이동의 시작 
    private Vector3 destinationPos; // Camera이동의 끝
    public bool isLandingSign;      // true로 바꾸면 Mesh가 올라감
    public GameObject TargetObject; // Mesh가 사라질 때 Mesh의 Object를 지우려는 변수
    public GameObject EffectObject; // Mesh가 사라질 때 Mesh의 Effect를 지우려는 변수

    const float initSize = 1.02f;   // Mesh가 초기에 올라가 있는 정도
    const float landingSize = 1.05f;// Mesh가 올라가는 정도

    private Material[] material;
    private Material beforeMat;

    static public int tmpDevCnt = 0;
    static public bool isMakeIsland = false;

    private MissionManager missionmanager;

    private float time_;
    private bool isTriggerOn = false;
    private MeshRenderer render;
    static public int cnt = 0;

    void Start ()
    {
        missionmanager = GameObject.FindWithTag("GameManager").GetComponent<MissionManager>();
        domMaterial = Resources.Load<Material>("M_Cold");
        priorMaterial = gameObject.GetComponent<Renderer>().material;
        priorState = gameObject.GetComponent<MeshController>().terrainstate;
        isAwake = false;
        isFixed = false;
        isMine = false;
        isLandingSign = false;
        temp = false;
        LinkedNumber = 0;

        transform.position *= initSize; // 초기 테두리 사이즈
        startPos = transform.position;
        destinationPos = transform.position * landingSize; // Landing 사이즈


    }

    public void SavedBeforeMat()
    {
        beforeMat = gameObject.GetComponent<MeshRenderer>().materials[0];
    }

    public void ReturnMat()
    {
        if (beforeMat == null)
            return;

        material = gameObject.GetComponent<MeshRenderer>().materials;
        material[0] = beforeMat;
        gameObject.GetComponent<MeshRenderer>().materials = material;

        beforeMat = null;
    }

    IEnumerator BlinkFlag()
    {
        render = gameObject.GetComponent<MeshRenderer>();
        render.material.shader = Shader.Find("Standard");

        isTriggerOn = true;
        time_ = 0;
        while(true)
        {
            time_ += Time.deltaTime;
            
            render.materials[0].SetColor("_Color", Color.Lerp(new Color32(255,255,255,255), new Color32(216,166,93,255), time_));
            yield return new WaitForEndOfFrame();
            if (time_ > 1.0f)
                break;
        }

        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            
            render.materials[0].SetColor("_Color", Color.Lerp(new Color32(216, 166, 93, 255), new Color32(255, 255, 255, 255), time_));
            yield return new WaitForEndOfFrame();
            if (time_ > 1.0f)
                break;
        }

        isTriggerOn = false;
    }

    void Update ()
    {
        if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() != null)
        {
            if (TutorialManager.index == 7 && isFlag && !isTriggerOn && gameObject.transform.childCount == 1)
            {
                StartCoroutine(BlinkFlag());
            }
        }
            

        if (isLandingSign == true)
        {
            InstateTerrainObject(terrainstate);
            //StartCoroutine(MoveUp());
            isLandingSign = false;
        }

        //if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
        //{
        //    if (isFlag && temp == false)
        //    {

        //        for (int i = 1; i < AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer.Length - 1; i++)
        //        {
        //            if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]) // 에러 나와서 일단 넣음 
        //            {
        //                float pTop = (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].transform.position - transform.position).magnitude;

        //                if (pTop < 19.45f && pTop != 0)
        //                {
        //                    if(AllMeshController.myPlanet.transform.childCount == 320)
        //                    {
        //                        AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().isFlagMesh = true;
        //                        NearMesh.Add(AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i]);
        //                        AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<MeshController>().priorMaterial = Resources.Load<Material>("M_JointFlag");
        //                    }

        //                    //Debug.Log(name);
        //                    //AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
        //                }
        //            }
        //        } // Flagable이면 주변 매쉬 받아와


        //        temp = true;
        //    }
        //}
        //else
        //{
        //    if (isFlag && temp == false)
        //    {

        //        for (int i = 1; i < TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer.Length - 1; i++)
        //        {
        //            if (TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer[i]) // 에러 나와서 일단 넣음 
        //            {
        //                float pTop = (TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer[i].transform.position - transform.position).magnitude;

        //                if (pTop < 19.45f && pTop != 0)
        //                {
        //                    TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer[i].GetComponent<MeshController>().isFlagMesh = true;
        //                    NearMesh.Add(TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer[i]);
        //                    TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().AllContainer[i].GetComponent<MeshController>().priorMaterial = Resources.Load<Material>("M_JointFlag");

        //                    //Debug.Log(name);
        //                    //AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[i].GetComponent<Renderer>().material = Resources.Load<Material>("M_JointFlag");
        //                }
        //            }
        //        } // Flagable이면 주변 매쉬 받아와


        //        temp = true;
        //    }
        //}

        

        if (isAwake)
        {
            if (true) // isMoblie == true
            {
                CameraController.lastmesh = MeshNumber;
                //Debug.Log("Pick : " + CameraController.lastmesh);
                int cameraYAmount = 138;
                Vector3 destinationPos = gameObject.transform.position.normalized * cameraYAmount;
                Camera.main.GetComponent<CameraController>().isCameraLock = true;
                StartCoroutine(Camera.main.GetComponent<CameraController>().movePosition(destinationPos));
            }
            
            if (terrainstate == Terrain.ABLE || terrainstate == Terrain.DEFAULT)
            {
                Picked();
            }
            else if (terrainstate == Terrain.FLAG)
            {
                if(GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() != null)
                {
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointOff();
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_SELECTTERRAINLOC);
                }
            }
            else
            {
                MissionCounting(false);

                GetComponent<MeshRenderer>().material = priorMaterial;
                terrainstate = priorState;
                for (int i = 0; i < 3; i++)
                {
                    if (JointMesh[i].GetComponent<MeshController>().terrainstate == Terrain.ABLE) // able일 때 default로 바꿔줘
                    {
                        if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                        {
                            for (int j = 0; j < AllMeshController.myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; j++)
                            {
                                if (AllMeshController.myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[j].name.
                                    Equals(JointMesh[i].GetComponent<MeshController>().name))
                                {
                                    Debug.Log("여기에 들어오나요??? [1]");
                                    JointMesh[i].GetComponent<MeshController>().setPriorState();
                                    JointMesh[i].GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_JointFlag");
                                    break;
                                }
                                else
                                {
                                    Debug.Log("여기에 들어오나요??? [2]");
                                    JointMesh[i].GetComponent<MeshController>().setPriorState();
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < AllMeshController.myPlanet.GetComponent<AllMeshController>().myFlag.GetComponent<MeshController>().NearMesh.Count; j++)
                            {
                                if (TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().myFlag.GetComponent<MeshController>().NearMesh[j].name.
                                    Equals(JointMesh[i].GetComponent<MeshController>().name))
                                {
                                    Debug.Log("여기에 들어오나요??? [3]");
                                    JointMesh[i].GetComponent<MeshController>().setPriorState();
                                    JointMesh[i].GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_JointFlag");
                                    break;
                                }
                                else
                                {
                                    Debug.Log("여기에 들어오나요??? [4]");
                                    JointMesh[i].GetComponent<MeshController>().setPriorState();
                                }
                            }
                        }
                            
                    }
                }
                CameraController.ChangeableCount++;
            }

            // Able 다시 세팅
            if(GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                AllMeshController.myPlanet.GetComponent<AllMeshController>().ResettingAble();
            else
            {
                TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().ResettingAble();
            }
                

            isAwake = false;
        }

        // 턴이 종료됐어 이번에 점령이 확정 됐다면 isFixed 를 true로 바꿔줘
    }
    
    public void InstateTerrainObject(Terrain terrainstate)
    {
        if (terrainObj != null && isFlag == false)
            Destroy(terrainObj);
        
        // 객체 추가해서 달아주자.
        if (terrainstate == Terrain.MODERATION)
        {
            //if(GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() != null)
            //{
            //    //움직이는 오브젝트가 나올때에는 생성 x
            //    if (TutorialFlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}
            //else
            //{
            //    //움직이는 오브젝트가 나올때에는 생성 x
            //    if (FlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}

            //EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[13 + cnt], 1.01f);
            //cnt += 1;
            //20퍼센트 확률로 생기지 않음.
            if (Random.Range(0, 100) < 80 || isFlag)
            {
                if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                    EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[RandomValue(13, 16)], 1.01f);
                else
                    EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[RandomValue(13, 16)], 1.01f);
            }
        }
        else if (terrainstate == Terrain.BARREN)
        {
            //if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() != null)
            //{
            //    //움직이는 오브젝트가 나올때에는 생성 x
            //    if (TutorialFlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}
            //else
            //{
            //    if (FlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}

            //70퍼센트 확률로 생기지 않음.
            if (Random.Range(0, 100) < 30 || isFlag)
            {
                if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                    EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[RandomValue(5, 12)], 1.01f);
                else
                    EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[RandomValue(5, 12)], 1.01f);
            }


        }

        else if (terrainstate == Terrain.COLD)
        {
            //if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() != null)
            //{
            //    //움직이는 오브젝트가 나올때에는 생성 x
            //    if (TutorialFlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}
            //
            //else
            //{
            //    //움직이는 오브젝트가 나올때에는 생성 x
            //    if (FlowSystem.finalTerrainName.Equals(gameObject.name))
            //        return;
            //}

            //40퍼센트 확률로 생기지 않음
            if (Random.Range(0, 100) < 60 || isFlag)
            {
                if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                    EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[RandomValue(1, 4)], 1.01f);
                else
                    EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[RandomValue(1, 4)], 1.01f);
            }

        }
        else if (terrainstate == Terrain.SEA)
        {
            int totalSum = 0;
            for (int i = 0; i < JointMesh.Length; i++)
            {
                if (JointMesh[i].GetComponent<MeshController>().terrainstate.Equals(Terrain.SEA))
                    totalSum += 1;
            }

            if (totalSum == 3 && !isMakeIsland)
            {
                isMakeIsland = true;
                if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                    EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[18], 1.01f);
                else
                    EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[18], 1.01f);
                return;
            }

            ////움직이는 오브젝트가 나올때에는 생성 x
            //if (FlowSystem.finalTerrainName.Equals(gameObject.name))
            //    return;

            //90퍼센트 확률로 생기지 않음.
            else if (Random.Range(0, 100) < 10 || isFlag)
            {
                if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                    EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[17], 1.01f);
                else
                    EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[17], 1.01f);
            }
        }
        else if (terrainstate == Terrain.MOUNTAIN)
        {
            for (int i = 0; i < JointMesh.Length; i++)
            {
                if (JointMesh[i].GetComponent<MeshController>().terrainstate.Equals(Terrain.MOUNTAIN))
                {
                    if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                        TargetObject = Instantiate(AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[19]);
                    else
                        TargetObject = Instantiate(TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[19]);

                    TargetObject.transform.position = ((gameObject.transform.position + JointMesh[i].transform.position) * 0.5f) * 1.01f;
                    TargetObject.transform.parent = GameObject.Find("ObjectSet").transform;

                    TargetObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);
                    TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x - 90, TargetObject.transform.eulerAngles.y, Random.Range(0, 360));
                }
            }
            if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>() == null)
                EulerRotCal(gameObject, AllMeshController.myPlanet.GetComponent<AllMeshController>().buildingObj[20], 1.01f);
            else
                EulerRotCal(gameObject, TutorialAllMeshController.myPlanet.GetComponent<TutorialAllMeshController>().buildingObj[20], 1.01f);
        }
        else if(terrainstate.Equals(Terrain.DEFAULT))
        {
            material = gameObject.GetComponent<MeshRenderer>().materials;
            material[0] = Resources.Load<Material>("M_Default");
            gameObject.GetComponent<MeshRenderer>().materials = material;
        }
    }

    public IEnumerator MoveUp()
    {
        while (transform.position.magnitude <= destinationPos.magnitude - 0.6f)
        {
            transform.position = Vector3.Lerp(transform.position, destinationPos, Time.deltaTime/2.0f);

            yield return null;
        }
    }

    public IEnumerator MoveDownCor()
    {
        while (transform.position.magnitude >= startPos.magnitude)
        {
            transform.position = Vector3.Lerp(transform.position, startPos * 1.0f, Time.deltaTime / 5);
            // 객체가 있었다면 객체들을 지워야 함.
            Destroy(TargetObject);
            yield return null;
        }
    }

    //재귀함수, 가장 아래에있는

    public void EulerRotCal(GameObject targetObj, GameObject buildingObj, float offset)
    {
        TargetObject = Instantiate(buildingObj);
        
        TargetObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        TargetObject.transform.parent = GameObject.Find("ObjectSet").transform;
        
        TargetObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);
        
        if(TargetObject.transform.childCount == 0)
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x - 90, TargetObject.transform.eulerAngles.y, Random.Range(0,360));
        else
        {
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x - 90, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
            TargetObject.transform.GetChild(0).transform.localEulerAngles = new Vector3(TargetObject.transform.GetChild(0).transform.localEulerAngles.x, TargetObject.transform.GetChild(0).transform.localEulerAngles.y, Random.Range(0, 360));
        }
        terrainObj = TargetObject;
    }

    public void EulerRotCalAltar(GameObject targetObj, GameObject buildingObj, float offset)
    {
        TargetObject = Instantiate(buildingObj);

        TargetObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        TargetObject.transform.parent = GameObject.Find("ObjectSet").transform;

        TargetObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);

        if (TargetObject.transform.childCount == 0)
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x - 90, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
        else
        {
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x - 90, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
        }
        terrainObj = TargetObject;
    }

    public void EulerRotCalAltar_A(GameObject targetObj, GameObject buildingObj, float offset)
    {
        TargetObject = Instantiate(buildingObj);

        TargetObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        TargetObject.transform.parent = GameObject.Find("ObjectSet").transform;

        TargetObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);

        if (TargetObject.transform.childCount == 0)
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x + 180, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
        else
        {
            TargetObject.transform.eulerAngles = new Vector3(TargetObject.transform.eulerAngles.x + 180, TargetObject.transform.eulerAngles.y, TargetObject.transform.eulerAngles.z);
        }
        terrainObj = TargetObject;
    }

    public void EulerRotCalEffect(GameObject targetObj, GameObject buildingObj, float offset)
    {
        EffectObject = Instantiate(buildingObj);

        EffectObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        EffectObject.transform.parent = GameObject.Find("ObjectSet").transform;

        EffectObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);

        if (EffectObject.transform.childCount == 0)
            EffectObject.transform.eulerAngles = new Vector3(EffectObject.transform.eulerAngles.x - 90, EffectObject.transform.eulerAngles.y, Random.Range(0, 360));
        else
        {
            EffectObject.transform.eulerAngles = new Vector3(EffectObject.transform.eulerAngles.x - 90, EffectObject.transform.eulerAngles.y, EffectObject.transform.eulerAngles.z);
            EffectObject.transform.GetChild(0).transform.localEulerAngles = new Vector3(EffectObject.transform.GetChild(0).transform.localEulerAngles.x, EffectObject.transform.GetChild(0).transform.localEulerAngles.y, Random.Range(0, 360));
        }
        terrainObj = EffectObject;

        StartCoroutine(deleteEffect(3.0f));
    }

    public IEnumerator deleteEffect(float time)
    {
        yield return new WaitForSeconds(time);

        Destroy(EffectObject);
    }

    public void MissionCounting(bool bVal)
    {
        if (isFlag == true) return;
        if (bVal)
        {
            if (currentIdentify.Equals(Identify.ENEMY))
            {
                //메인미션 110번, 상성지형 상대지형 50칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 3)
                    missionmanager.MainMissionContinuedCounting(1);

                //서브미션 220번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 0)
                    missionmanager.SubMissionContinuedCounting(1, 4);

                //서브미션 320번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 1)
                    missionmanager.SubMissionContinuedCounting(1, 3);

                //서브미션 410번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 2)
                    missionmanager.SubMissionContinuedCounting(1, 2);

                //서브미션 510번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 3)
                    missionmanager.SubMissionContinuedCounting(1, 2);
            }
        }
        else
        {
            if (currentIdentify.Equals(Identify.ENEMY))
            {
                //메인미션 110번, 상성지형 상대지형 50칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 3)
                    missionmanager.MainMissionContinuedCounting(-1);

                //서브미션 220번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 0)
                    missionmanager.SubMissionContinuedCounting(-1, 4);

                //서브미션 320번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 1)
                    missionmanager.SubMissionContinuedCounting(-1, 3);

                //서브미션 410번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 2)
                    missionmanager.SubMissionContinuedCounting(-1, 2);

                //서브미션 510번, 상성지형 상대지형 30칸이상 점령
                if (MissionManager.selectedSubMissionIndex == 3)
                    missionmanager.SubMissionContinuedCounting(-1, 2);
            }
        }
    }

    public void Picked()
    {
        //튜토리얼일경우
        if (GameObject.FindWithTag("GameManager").GetComponent<TutorialCardSystem>() != null)
        {
            if (TutorialAllMeshController.IngameManager.GetComponent<TutorialCardSystem>().pickedCard)
            {
                GameObject picked = TutorialAllMeshController.IngameManager.GetComponent<TutorialCardSystem>().pickedCard;

                if (TutorialManager.index == 9)
                {
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(13, -6, 0));
                    TutorialPcVerCamera.inputAble = 185;
                }

                else if (TutorialManager.index == 10)
                {
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(88, 27, 0));
                    TutorialPcVerCamera.inputAble = 124;
                }

                else if (TutorialManager.index == 11)
                {
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(13, 71, 0));
                    TutorialPcVerCamera.inputAble = 154;
                }

                else if (TutorialManager.index == 12)
                {
                    TutorialPcVerCamera.inputAble = 159;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-10, 88, 0));
                }
                    
                else if (TutorialManager.index == 13)
                {
                    TutorialPcVerCamera.inputAble = 36;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(17, 88, 0));
                }
                    
                else if (TutorialManager.index == 14)
                {
                    TutorialPcVerCamera.inputAble = 183;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-17, 74, 0));
                }
                    
                else if (TutorialManager.index == 15)
                {
                    TutorialPcVerCamera.inputAble = 147;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-78, 37, 0));
                }
                    
                else if (TutorialManager.index == 16)
                {
                    TutorialPcVerCamera.inputAble = 92;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-17, -10, 0));
                }
                    
                else if (TutorialManager.index == 17)
                {
                    TutorialPcVerCamera.inputAble = 31;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-78, 27, 0));
                }
                    
                else if (TutorialManager.index == 18)
                {
                    TutorialPcVerCamera.inputAble = 310;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-17, -24, 0));
                }
                    
                else if (TutorialManager.index == 19)
                {
                    TutorialPcVerCamera.inputAble = 49;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(12, -25, 0));
                }
                    
                else if(TutorialManager.index == 20)
                {
                    TutorialPcVerCamera.inputAble = -1;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointOff();
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_SELECTLOCDONE);
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].transform.localPosition = new Vector3(525, -195, 0);
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].transform.localScale = new Vector3(2.58f, 2.58f, 2.58f);
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].SetActive(true);
                }

                else if (TutorialManager.index == 23)
                {
                    TutorialPcVerCamera.inputAble = 251;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-13, -19, 0));
                }
                    
                else if (TutorialManager.index == 24)
                {
                    TutorialPcVerCamera.inputAble = 195;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(9, -23, 0));
                }
                    
                else if (TutorialManager.index == 25)
                {
                    TutorialPcVerCamera.inputAble = 198;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-23, -20, 0));
                }
                    
                else if (TutorialManager.index == 26)
                {
                    TutorialPcVerCamera.inputAble = 93;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-67, 31, 0));
                }
                    
                else if (TutorialManager.index == 27)
                {
                    TutorialPcVerCamera.inputAble = 273;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-5, 82, 0));
                }
                    
                else if (TutorialManager.index == 28)
                {
                    TutorialPcVerCamera.inputAble = 21;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(24, 83, 0));
                }
                    
                else if (TutorialManager.index == 29)
                {
                    TutorialPcVerCamera.inputAble = -1;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointOff();
                }
                    

                else if (TutorialManager.index == 32)
                {
                    TutorialPcVerCamera.inputAble = 257;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(6, 92, 0));
                }
                    
                else if (TutorialManager.index == 33)
                {
                    TutorialPcVerCamera.inputAble = 233;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(-34, 70, 0));
                }
                    
                else if (TutorialManager.index == 34)
                {
                    TutorialPcVerCamera.inputAble = 111;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(13, 89, 0));
                }
                    
                else if (TutorialManager.index == 35)
                {
                    TutorialPcVerCamera.inputAble = 39;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().PointArrowMove(new Vector3(57, 44, 0));
                }
                    
                else if (TutorialManager.index == 36)
                {
                    TutorialPcVerCamera.inputAble = -1;
                    GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointOff();
                }
                    

                CameraController.ChangeableCount--;

                MissionCounting(true);

                if (picked.name.Equals("TerrainCardImg1"))
                {
                    setModeration(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg2"))
                {
                    setBarren(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg3"))
                {
                    setCold(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg4"))
                {
                    setSea(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg5"))
                {
                    setMountain(Identify.ALLY);
                }
            }
        }

        //튜토리얼이 아닐경우
        else
        {
            if (AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard)
            {
                GameObject picked = AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard;

                CameraController.ChangeableCount--;

                MissionCounting(true);

                if (picked.name.Equals("TerrainCardImg1"))
                {
                    setModeration(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg2"))
                {
                    setBarren(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg3"))
                {
                    setCold(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg4"))
                {
                    setSea(Identify.ALLY);
                }
                else if (picked.name.Equals("TerrainCardImg5"))
                {
                    setMountain(Identify.ALLY);
                }
            }
        }
        
    }

    private int RandomValue(int min, int max)
    {
        return Random.Range(min, max + 1);
    }

    //적당
    public void setModeration(Identify identify)
    {
        terrainstate = Terrain.MODERATION;

        if (identify.Equals(Identify.ALLY))
        {
            domMaterial = Resources.Load<Material>("M_Moderation");
            currentIdentify = Identify.ALLY;
        }
        else if (identify.Equals(Identify.ENEMY))
        {
            domMaterial = Resources.Load<Material>("M_ModerationEnemy");
            currentIdentify = Identify.ENEMY;
        }

        GetComponent<MeshRenderer>().material = domMaterial;
    }
    //건조
    public void setBarren(Identify identify)
    {
        terrainstate = Terrain.BARREN;

        if (identify.Equals(Identify.ALLY))
        {
            domMaterial = Resources.Load<Material>("M_Barren");
            currentIdentify = Identify.ALLY;
        }
        else if (identify.Equals(Identify.ENEMY))
        {
            domMaterial = Resources.Load<Material>("M_BarrenEnemy");
            currentIdentify = Identify.ENEMY;
        }

        GetComponent<MeshRenderer>().material = domMaterial;
    }
    //한랭
    public void setCold(Identify identify)
    {
        terrainstate = Terrain.COLD;

        if (identify.Equals(Identify.ALLY))
        {
            domMaterial = Resources.Load<Material>("M_Cold");
            currentIdentify = Identify.ALLY;
        }
        else if (identify.Equals(Identify.ENEMY))
        {
            domMaterial = Resources.Load<Material>("M_ColdEnemy");
            currentIdentify = Identify.ENEMY;
        }

        GetComponent<MeshRenderer>().material = domMaterial;
    }

    public void setSea(Identify identify)
    {
        terrainstate = Terrain.SEA;

        if (identify.Equals(Identify.ALLY))
        {
            domMaterial = Resources.Load<Material>("M_Sea");
            currentIdentify = Identify.ALLY;
        }
        else if (identify.Equals(Identify.ENEMY))
        {
            domMaterial = Resources.Load<Material>("M_SeaEnemy");
            currentIdentify = Identify.ENEMY;
        }

        GetComponent<MeshRenderer>().material = domMaterial;
    }

    public void setMountain(Identify identify)
    {
        terrainstate = Terrain.MOUNTAIN;

        if (identify.Equals(Identify.ALLY))
        {
            domMaterial = Resources.Load<Material>("M_Mountain");
            currentIdentify = Identify.ALLY;
        }
        else if (identify.Equals(Identify.ENEMY))
        {
            domMaterial = Resources.Load<Material>("M_MountainEnemy");
            currentIdentify = Identify.ENEMY;
        }

        GetComponent<MeshRenderer>().material = domMaterial;
    }

    public void setFlag(Identify identify)
    {
        terrainstate = Terrain.FLAG;

        if (identify.Equals(Identify.ALLY))
        {
            currentIdentify = Identify.ALLY;
        }
        else
        {
            currentIdentify = Identify.ENEMY;
        }

        domMaterial = Resources.Load<Material>("M_FlagAble");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
    }

    public void setDefault()
    {
        terrainstate = Terrain.DEFAULT;
        domMaterial = Resources.Load<Material>("M_Default");
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
        currentIdentify = Identify.NEUTRALITY;
    }

    public void setPriorState()
    {
        terrainstate = priorState;
        domMaterial = priorMaterial;
        GetComponent<MeshRenderer>().material = domMaterial; // 지금 머테리얼을 바꿔줌 // 머테리얼은 선택된 지형카드에 따라
        currentIdentify = Identify.NEUTRALITY;
    }
}