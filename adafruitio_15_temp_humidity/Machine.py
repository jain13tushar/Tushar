import pandas as pd

def machine():
    train = pd.read_csv('result.csv')

    train.drop('date_time',axis=1,inplace=True) #axis=1 to remove column //change needs to be permanent

    train.dropna(inplace=True) #for missing the values

    X = train.drop('Fan_value', axis=1) #pedict the value of fan based on Humdity and temperature
    y = train['Fan_value']# for the output

    from sklearn.model_selection import train_test_split

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33)#

    from sklearn.linear_model import LogisticRegression

    logmodel = LogisticRegression()#importing into the variable

    logmodel.fit(X_train, y_train)

    predictions = logmodel.predict(X_test)

    from sklearn.metrics import classification_report

#print(classification_report(y_test,predictions))

    #print(predictions)
    return predictions