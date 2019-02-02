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
    private Terrain selectedTerrain;

    private GameObject connectedObj;
    private int tmpCardIndex;

    private bool[] check = new bool[6];

    private List<GameObject> connectObjs = new List<GameObject>();

    public GameObject conditionABtn;
    public GameObject conditionBBtn;

    private void Start()
    {
        for (int i = 0; i < check.Length; i++)
            check[i] = false;
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

    public void EventCardInstate()
    {
        RandomEventCardSelect(Random.Range(0, 6));

        eventCard.SetActive(true);
        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        conditionABtn.SetActive(false);
        conditionBBtn.SetActive(false);

        if (pickedCard.cardIndex == 301)
            conditionBBtn.SetActive(true);
        else
            conditionABtn.SetActive(true);

        //선택된 카드 서버로 보내기
        //pickedCard.data.cardIndex
    }

    IEnumerator UseCardCor()
    {
        switch (selectedIndex)
        {
            //지형파괴 - 타겟 : 적
            case 101:
                gameObject.GetComponent<FlowSystem>().displayText.GetComponent<DisplayText>().text = "파괴할 상대의 구역을 선택하세요.";
                gameObject.GetComponent<FlowSystem>().displayText.SetActive(true);
                break;
            //소유권 전환 - 타겟 : 적
            case 111:
                gameObject.GetComponent<FlowSystem>().displayText.GetComponent<DisplayText>().text = "소유권을 전환할 상대의 구역을 선택하세요.";
                gameObject.GetComponent<FlowSystem>().displayText.SetActive(true);
                break;
            //속성변경 - 타겟 : 나
            case 201:
                gameObject.GetComponent<FlowSystem>().displayText.GetComponent<DisplayText>().text = "속성 변경 할 나의 구역을 선택하세요.";
                gameObject.GetComponent<FlowSystem>().displayText.SetActive(true);
                break;
                //속성변경 - 타겟 : 적
            case 202:
                gameObject.GetComponent<FlowSystem>().displayText.GetComponent<DisplayText>().text = "속성 변경 할 상대의 구역을 선택하세요.";
                gameObject.GetComponent<FlowSystem>().displayText.SetActive(true);
                break;
        }
        yield return new WaitForSeconds(2.5f);
        gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
    }

    //사용 할 경우
    public void UseCard()
    {
        eventCard.SetActive(false);

        switch (selectedIndex)
        {
            //특수카드 방어
            case 301:
                TurnSystem.enemyEventCardDefense = true;
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            //주사위 두배
            case 401:
                DiceSystem.instance_.gameObject.transform.parent.gameObject.SetActive(true);
                DiceSystem.instance_.doubleImg.SetActive(true);
                DiceSystem.instance_.doubleImg.AddComponent<IsDoubleImg>();
                DiceSystem.isDiceDouble = true;
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            default:
                StartCoroutine(UseCardCor());
                break;
        }
    }

    public void DontUseCard()
    {
        eventCard.SetActive(false);
    }

    public void PickLocDone(GameObject obj, Terrain terrainType)
    {
        switch (selectedIndex)
        {
            case 101:
                ConnectObj(obj, terrainType);
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setDefault();
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.DEFAULT);
                    StartCoroutine(connectObjs[i].GetComponent<MeshController>().MoveDownCor());

                }
                    
                connectObjs.Clear();
                AllMeshController.IngameManager.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            case 111:
                ConnectObj(obj, terrainType);
                for (int i = 0; i < connectObjs.Count; i++)
                    connectObjs[i].GetComponent<MeshController>().currentIdentify = Identify.ALLY;
                connectObjs.Clear();
                AllMeshController.IngameManager.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            case 201:
                ConnectObj(obj, terrainType);
                selectTerrainCardObj.SetActive(true);
                AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTSECLECTTERRAIN;
                break;
            case 202:
                ConnectObj(obj, terrainType);
                selectTerrainCardObj.SetActive(true);
                AllMeshController.IngameManager.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTSECLECTTERRAIN;
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

        switch(selectedTerrain)
        {
            case Terrain.MODERATION:
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setModeration(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.MODERATION);
                }
                    
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[0], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
            case Terrain.BARREN:
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setBarren(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.BARREN);
                }
                    
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[6], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
            case Terrain.COLD:
                for (int i = 0; i < connectObjs.Count; i++)
                {
                    connectObjs[i].GetComponent<MeshController>().setCold(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                    connectObjs[i].GetComponent<MeshController>().InstateTerrainObject(Terrain.COLD);
                }
                    
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[Random.Range(1,4)], 1.03f, int.Parse(connectObjs[0].name), tmpCardIndex);
                break;
        }

        selectTerrainCardObj.SetActive(false);
        connectObjs.Clear();
        AllMeshController.IngameManager.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
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
