using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EventCardManager : MonoBehaviour
{
    public CardInfo[] eventCardInfoSet;
    private CardInfo pickedCard;

    public GameObject eventCard;
    public Text cardName;
    public Image cardImg;
    public GameObject selectTerrainCardObj;
    static public int selectedIndex;
    static public bool isDefenseCardGet = false;
    private Terrain selectedTerrain;

    private GameObject connectedObj;
    private int tmpCardIndex;

    private bool[] check = new bool[6];

    private List<GameObject> connectObjs = new List<GameObject>();

    public GameObject conditionABtn;
    public GameObject conditionBBtn;

    private Material[] material;

    //최적화 스크립트
    private FlowSystem flowsystem;
    private Transform planetTrans;
    private MissionManager missionManager;
    private BuildOnPlanet buildOnPlanet;

    int[] toServerIntArray;

    private void Start()
    {
        for (int i = 0; i < check.Length; i++)
            check[i] = false;

        flowsystem = gameObject.GetComponent<FlowSystem>();
        planetTrans = GameObject.FindWithTag("InGamePlanet").transform;
        missionManager = gameObject.GetComponent<MissionManager>();
        buildOnPlanet = gameObject.GetComponent<BuildOnPlanet>();
    }

    public void InstateEnemyEventCard(int index)
    {
        eventCard.SetActive(true);
        pickedCard = eventCardInfoSet[index];

        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        conditionABtn.SetActive(false);
        conditionBBtn.SetActive(false);
    }
    public void EnemyEventCardOff()
    {
        eventCard.SetActive(false);
    }

    public void RandomEventCardSelect(int index)
    {
        if (check[index])
        {
            RandomEventCardSelect(Random.Range(0, 6));
            return;
        }


        if (!PCverPIcking.isDominatedConfirm && index == 0)
        {
            RandomEventCardSelect(Random.Range(0, 6));
            return;
        }

        check[index] = true;
        pickedCard = eventCardInfoSet[index];
        int cnt = 0;
        for(int i =0; i<check.Length;i++)
        {
            if (check[i])
                cnt += 1;
        }
        if(cnt == check.Length)
        {
            for (int i = 0; i < check.Length; i++)
                check[i] = false;
        }
    }

    public void DefenseCard()
    {
        pickedCard = eventCardInfoSet[4];

        eventCard.SetActive(true);
        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        conditionABtn.SetActive(false);
        conditionBBtn.SetActive(true);
    }

    public void EventCardInstate()
    {
        RandomEventCardSelect(Random.Range(0, 6));

        eventCard.SetActive(true);
        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        conditionBBtn.SetActive(false);
        conditionABtn.SetActive(true);

        //선택된 카드 서버로 보내기
        if (GameObject.Find("NetworkManager") != null)
            GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventcardIndex(pickedCard.cardIndex);
    }

    //서버에서 전달받은거 변경.
    public void MeshInfoUpdate(int eventIndex, int terrainIndex, int cardIndex, bool isDefense)
    {
        switch(eventIndex)
        {
            //지형_파괴 
            case 101:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (planetTrans.GetChild(i).GetComponent<MeshController>().MeshNumber == terrainIndex)
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().setDefault();
                        planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.DEFAULT);
                        break;
                    }
                }
                break;

            //소유권_전환
            case 111:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (planetTrans.GetChild(i).GetComponent<MeshController>().MeshNumber == terrainIndex)
                    {
                        connectObjs[i].GetComponent<MeshController>().currentIdentify = Identify.ENEMY;
                        //메테리얼도 바꿔야함
                        break;
                    }
                }
                break;

            //내_지형_속성_변경
            case 201:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (planetTrans.GetChild(i).GetComponent<MeshController>().MeshNumber == terrainIndex)
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().setBarren(planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify);
                        if(cardIndex == 1)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.MODERATION);
                        else if(cardIndex == 2)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.BARREN);
                        else if(cardIndex == 3)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.COLD);

                        //buildOnPlanet.EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[6], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                        break;
                    }
                }
                break;

            //상대_지형_속성_변경
            case 202:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (planetTrans.GetChild(i).GetComponent<MeshController>().MeshNumber == terrainIndex)
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().setBarren(planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify);
                        if (cardIndex == 1)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.MODERATION);
                        else if (cardIndex == 2)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.BARREN);
                        else if (cardIndex == 3)
                            planetTrans.GetChild(i).GetComponent<MeshController>().InstateTerrainObject(Terrain.COLD);

                        //buildOnPlanet.EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[6], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                        break;
                    }
                }
                break;

            //특수_카드_방어
            case 301:
                // True면 사용, False면 사용하지 않음.
                if(isDefense)
                {
                    flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                }
                    
                break;

            //주사위_두배
            case 401:
                // 불리지 않음.
                break;
        }
        
    }

    IEnumerator UseCardCor()
    {
        switch (selectedIndex)
        {
            //지형파괴 - 타겟 : 적
            case 101:
                for(int i =0; i<planetTrans.childCount;i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().SavedBeforeMat();
                        material = planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials;
                        material[0] = Resources.Load<Material>("M_Cannot");
                        planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials = material;
                    }
                }
                flowsystem.displayText.GetComponent<DisplayText>().text = "파괴할 상대의 구역을 선택하세요.";
                flowsystem.displayText.SetActive(true);
                break;
            //소유권 전환 - 타겟 : 적
            case 111:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().SavedBeforeMat();
                        material = planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials;
                        material[0] = Resources.Load<Material>("M_Cannot");
                        planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials = material;
                    }
                }
                flowsystem.displayText.GetComponent<DisplayText>().text = "소유권을 전환할 상대의 구역을 선택하세요.";
                flowsystem.displayText.SetActive(true);
                break;
            //속성변경 - 타겟 : 나
            case 201:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ALLY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().SavedBeforeMat();
                        material = planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials;
                        material[0] = Resources.Load<Material>("M_Cannot");
                        planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials = material;
                    }
                }
                flowsystem.displayText.GetComponent<DisplayText>().text = "속성 변경 할 나의 구역을 선택하세요.";
                flowsystem.displayText.SetActive(true);
                break;
                //속성변경 - 타겟 : 적
            case 202:
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().SavedBeforeMat();
                        material = planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials;
                        material[0] = Resources.Load<Material>("M_Cannot");
                        planetTrans.GetChild(i).GetComponent<MeshRenderer>().materials = material;
                    }
                }
                flowsystem.displayText.GetComponent<DisplayText>().text = "속성 변경 할 상대의 구역을 선택하세요.";
                flowsystem.displayText.SetActive(true);
                break;
        }
        yield return new WaitForSeconds(2.5f);
        flowsystem.currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
    }

    //방어카드는 지금쓰는게아니라 소지하고있어야함.
    public void GetCard()
    {
        if (selectedIndex == 301)
        {
            isDefenseCardGet = true;
            flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
        }
        else
            UseCard();
    }

    //사용 할 경우
    public void UseCard()
    {
        eventCard.SetActive(false);
       
        switch (selectedIndex)
        {
            //특수카드 방어
            case 301:
                isDefenseCardGet = false;
                //미션 - 330
                if (MissionManager.selectedSubMissionIndex == 1)
                    missionManager.SubMissionCounting(1, 4);

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(true, null, -1);

                flowsystem.FlowChange(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC);
                break;
            //주사위 두배
            case 401:
                DiceSystem.instance_.gameObject.transform.parent.gameObject.SetActive(true);
                DiceSystem.instance_.doubleImg.SetActive(true);
                DiceSystem.instance_.doubleImg.AddComponent<IsDoubleImg>();
                DiceSystem.isDiceDouble = true;

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, null, -1);

                flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            default:
                StartCoroutine(UseCardCor());
                break;
        }
    }
    
    public void DontUseCard()
    {
        eventCard.SetActive(false);

        //서버로 데이터 전송
        if (GameObject.Find("NetworkManager") != null)
            GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, null, -1);

        flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
    }
    
    public void PickLocDone(GameObject obj, Terrain terrainType)
    {
        switch (selectedIndex)
        {
            //지형파괴 - 타겟 : 적
            case 101:
                ConnectObj(obj, terrainType);
                int[] toServerIntArray = new int[connectObjs.Count];
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setDefault();
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.DEFAULT);
                    //StartCoroutine(connectObjs[i].GetComponent<MeshController>().MoveDownCor());

                    toServerIntArray[i] = connectObjs[i].GetComponent<MeshController>().MeshNumber;
                }

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, toServerIntArray, -1);

                connectObjs.Clear();
                flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().ReturnMat();
                    }
                }

               

                break;
                //소유권전환 - 타겟 : 적
            case 111:
                ConnectObj(obj, terrainType);
                toServerIntArray = new int[connectObjs.Count];

                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().currentIdentify = Identify.ALLY;
                    //메쉬 메테리얼도 바꿔줘야함

                    toServerIntArray[i] = connectObjs[i].GetComponent<MeshController>().MeshNumber;
                }

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, toServerIntArray, -1);

                connectObjs.Clear();
                flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().ReturnMat();
                    }
                }
                break;
                //속성변경 - 타겟 : 나
            case 201:
                ConnectObj(obj, terrainType);
                selectTerrainCardObj.SetActive(true);
                flowsystem.currentFlow = FLOW.TO_PICKINGEVENTSECLECTTERRAIN;
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ALLY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().ReturnMat();
                    }
                }
                break;
                //속성변경 - 타겟 : 적
            case 202:
                ConnectObj(obj, terrainType);
                selectTerrainCardObj.SetActive(true);
                flowsystem.currentFlow = FLOW.TO_PICKINGEVENTSECLECTTERRAIN;
                for (int i = 0; i < planetTrans.childCount; i++)
                {
                    if (!planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(Identify.ENEMY))
                    {
                        planetTrans.GetChild(i).GetComponent<MeshController>().ReturnMat();
                    }
                }
                break;
        }
    }

    private bool SearchList(GameObject obj)
    {
        for (int i = 0; i < connectObjs.Count; i++)
        {
            if (connectObjs[i].Equals(obj))
                return true;
        }
        return false;
    }

    public void SelectTerrain(int type)
    {
        switch(type)
        {
            case 1:
                selectedTerrain = Terrain.MODERATION;
                tmpCardIndex = 1;
                break;
            case 2:
                selectedTerrain = Terrain.BARREN;
                tmpCardIndex = 2;
                break;
            case 3:
                selectedTerrain = Terrain.COLD;
                tmpCardIndex = 3;
                break;
        }

        switch (selectedTerrain)
        {
            case Terrain.MODERATION:
                toServerIntArray = new int[connectObjs.Count];
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setModeration(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.MODERATION);

                    toServerIntArray[i] = connectObjs[i].GetComponent<MeshController>().MeshNumber;
                }

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, toServerIntArray, tmpCardIndex);

                buildOnPlanet.EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[0], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
            case Terrain.BARREN:
                toServerIntArray = new int[connectObjs.Count];
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setBarren(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.BARREN);

                    toServerIntArray[i] = connectObjs[i].GetComponent<MeshController>().MeshNumber;

                }

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, toServerIntArray, tmpCardIndex);

                buildOnPlanet.EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[6], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
            case Terrain.COLD:
                toServerIntArray = new int[connectObjs.Count];
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setCold(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.COLD);

                    toServerIntArray[i] = connectObjs[i].GetComponent<MeshController>().MeshNumber;
                }

                //서버로 데이터 전송
                if (GameObject.Find("NetworkManager") != null)
                    GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().SendEventBuffer(false, toServerIntArray, tmpCardIndex);

                buildOnPlanet.EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[Random.Range(1,4)], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
        }

        selectTerrainCardObj.SetActive(false);
        connectObjs.Clear();
        flowsystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
    }

    public void CheckingMesh(GameObject obj, Terrain terrainType)
    {
        for (int i = 0; i < 3; i++)
        {
            if (obj.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().isCheck)
                continue;
            else
            {
                if (obj.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().terrainstate.Equals(terrainType))
                    ConnectObj(obj.GetComponent<MeshController>().JointMesh[i], terrainType);
            }
        }
    }

    public void ConnectObj(GameObject obj, Terrain terrainType)
    {
        if (!SearchList(obj))
        {
            connectObjs.Add(obj);

            for (int i = 0; i < 3; i++)
            {
                connectedObj = obj.GetComponent<MeshController>().JointMesh[i];
                if (connectedObj.GetComponent<MeshController>().terrainstate.Equals(terrainType))
                {
                    if (!obj.GetComponent<MeshController>().isCheck)
                        CheckingMesh(obj, terrainType);
                    else
                        obj.GetComponent<MeshController>().isCheck = true;
                }
                else
                    continue;
            }
        }
    }
}
