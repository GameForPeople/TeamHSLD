using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    float rotate = 0;
    GameObject EffectObject;

    // Use this for initialization
    void Start () {
        //setCard(cardObject[0]);
    }
	
	// Update is called once per frame
	void Update () {

    }

    void setCard(GameObject cardObj)
    {
        Vector3 firstPosition = new Vector3(0, 0, 0);
        Quaternion firstQuaternion = new Quaternion(5, 0, 0, 0);
        EffectObject = Instantiate(cardObj, firstPosition, firstQuaternion);
        Debug.Log("Create");

        StartCoroutine(movingCard(cardObj));
    }
    
    public IEnumerator movingCard(GameObject cardObj)
    {
        while (rotate < 360)
        {
            rotate += Time.deltaTime;

            Debug.Log(rotate);
            cardObj.transform.RotateAround(
                gameObject.GetComponent<Transform>().position,
                Vector3.left, 1);

            yield return null;
        }
    }
}
