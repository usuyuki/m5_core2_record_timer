/** @format */

function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("シート1");

  // 送信されてくるJSONデータ　{"ID":XXXX,"temp":XXXXX}　から、各要素を取り出す
  var params = JSON.parse(e.postData.getDataAsString());
  var time = params.time;
  var type = params.type;

  // データをシートに追加
  sheet.insertRows(2, 1); // 2行1列目に列を挿入する
  sheet.getRange(2, 1).setValue(new Date()); // 受信日時を記録
  sheet.getRange(2, 2).setValue(time); // IDを記録
  sheet.getRange(2, 3).setValue(type); // tempを記録
}
