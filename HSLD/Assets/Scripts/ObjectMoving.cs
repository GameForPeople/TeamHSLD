using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum MOVINGOBJECT
{
    PENGUIN,
    WHALE,
    CHICKEN
}

public class ObjectMoving : MonoBehaviour
{
    public MOVINGOBJECT thisObject;
    private GameObject[] ableToGoObj = new GameObject[3];
    public int startingIndex;
    private int randomVal;
    private float time_;

    private void Start()
    {
        StartCoroutine(Moving(GameObject.Find(startingIndex.ToString())));
    }

    void AlbeToGo(GameObject obj)
    {
        int j = 0;
        for(int i =0; i< obj.GetComponent<MeshController>().JointMesh.Length;i++)
        {
            if (obj.GetComponent<MeshController>().terrainstate.Equals(obj.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().terrainstate))
            {
                ableToGoObj[j] = obj.GetComponent<MeshController>().JointMesh[i];
                j += 1;
            }
        }
    }

    IEnumerator Moving(GameObject startingPoint)
    {
        AlbeToGo(startingPoint);
        randomVal = Random.Range(0, 3);

        if (ableToGoObj[randomVal] == null)
            StartCoroutine(Moving(startingPoint));
        else
        {
            time_ = 0;
            while (true)
            {
                time_ += Time.deltaTime * 0.5f;
                gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position + new Vector3(0, 1.03f, 0), ableToGoObj[randomVal].transform.position + new Vector3(0, 1.03f, 0), time_);
                gameObject.transform.LookAt(GameObject.Find("InGameSceneManager").transform);
                gameObject.transform.eulerAngles = new Vector3(gameObject.transform.eulerAngles.x + 180, gameObject.transform.eulerAngles.y, gameObject.transform.eulerAngles.z); 

                yield return new WaitForEndOfFrame();
                if (time_ > 1f)
                    break;
            }

            StartCoroutine(Moving(ableToGoObj[randomVal]));
        }
        
    }
}
