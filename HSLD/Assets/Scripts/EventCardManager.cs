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

    private List<GameObject> connectObjs = new List<GameObject>();

    public void EventCardInstate()
    {
        pickedCard = eventCardInfoSet[Random.Range(2, 3)];
        eventCard.SetActive(true);

        cardName.text = pickedCard.cardName;
        cardImg.sprite = pickedCard.img;
        selectedIndex = pickedCard.cardIndex;

        //선택된 카드 서버로 보내기
        //pickedCard.data.cardIndex
    }

    //사용 할 경우
    public void UseCard()
    {
        eventCard.SetActive(false);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);

        switch(selectedIndex)
        {
            case 101:
                gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
                break;
            case 111:
                gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
                break;
            case 201:
                gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
                break;
            case 202:
                gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_PICKINGEVENTCARDLOC;
                break;
            case 301:
                TurnSystem.enemyEventCardDefense = true;
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
            case 401:
                DiceSystem.isDiceDouble = true;
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);
                break;
        }
    }

    public void PickLocDone(GameObject obj, Terrain terrainType)
    {
        switch (selectedIndex)
        {
            case 101:
                ConnectObj(obj, terrainType);
                for (int i = 0; i < connectObjs.Count; i++)
                    connectObjs[i].GetComponent<MeshController>().currentIdentify = Identify.NEUTRALITY;
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
                break;
            case 2:
                selectedTerrain = Terrain.BARREN;
                break;
            case 3:
                selectedTerrain = Terrain.COLD;
                break;
        }

        switch(selectedTerrain)
        {
            case Terrain.MODERATION:
                for (int i = 0; i < connectObjs.Count; i++)
                    connectObjs[i].GetComponent<MeshController>().setModeration(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[0], 1.03f, int.Parse(connectObjs[0].name), gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                break;
            case Terrain.BARREN:
                for (int i = 0; i < connectObjs.Count; i++)
                    connectObjs[i].GetComponent<MeshController>().setBarren(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[6], 1.03f, int.Parse(connectObjs[0].name), gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                break;
            case Terrain.COLD:
                for (int i = 0; i < connectObjs.Count; i++)
                    connectObjs[i].GetComponent<MeshController>().setCold(connectObjs[i].GetComponent<MeshController>().currentIdentify);
                gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(connectObjs[0], AllMeshController.instance_.MovingObj[Random.Range(1,4)], 1.03f, int.Parse(connectObjs[0].name), gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
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
