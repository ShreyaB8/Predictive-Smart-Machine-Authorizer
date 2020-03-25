const functions = require('firebase-functions');
const admin = require('firebase-admin');
var AWS = require('aws-sdk');
AWS.config.update({region: 'REGION'});

var sqs = new AWS.SQS({apiVersion: '2012-11-05'});
admin.initializeApp({
    databaseURL: "https://digilocker-dc8c1.firebaseio.com/"
  });
  
var db = admin.database();
var ref = db.ref("Locker/L001");

ref.on("child_changed", function(snapshot) {
    var changedPost = snapshot.val();
    console.log("locker status changed" + changedPost.locked);
  }, function (errorObject) {
    console.log("The read failed: " + errorObject.code);
});

var params = {
  DelaySeconds: 10,
  MessageAttributes: {
    "locker_status": {
      locker_status: changedPost
    }
  },
  MessageBody: "Locker status sent",
  QueueUrl: "SQS_QUEUE_URL"
};

sqs.sendMessage(params, function(err, data) {
  if (err) {
    console.log("Error", err);
  } else {
    console.log("Success", data.MessageId);
  }
});
  