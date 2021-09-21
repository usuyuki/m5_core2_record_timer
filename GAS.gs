/** @format */

function doPost(e) {
  var sheet =
    SpreadsheetApp.getActiveSpreadsheet().getSheetByName("main_sheet");

  // 送信されてくるJSONデータがeに入り、json.parseで取り出し
  var params = JSON.parse(e.postData.getDataAsString());
  var studyType = params.type;
  var studyTime = params.time;
  var now_time = new Date();

  var old_time = sheet.getRange("A2").getValue();
  // データをシートに追加
  //その日がなかったらその日の項目を作る
  if (
    now_time.getFullYear() === old_time.getFullYear() &&
    now_time.getMonth() === old_time.getMonth() &&
    now_time.getDate() === old_time.getDate()
  ) {
    //既に同日があるとき
  } else {
    //同日がないとき
    sheet.insertRows(2, 1); // 列を挿入する
    sheet.getRange("A2").setValue(new Date()); // 現在時刻
    sheet.getRange("B2").setValue(0);
    sheet.getRange("C2").setValue(0);
    sheet.getRange("D2").setValue(0);
    sheet.getRange("E2").setValue(0);
    sheet.getRange("F2").setValue(0);
  }
  //タイプに応じで分ける＋元の数にプラスする
  if (studyType == "AmateurRadio") {
    sheet
      .getRange("B2")
      .setValue(Number(sheet.getRange("B2").getValue()) + Number(studyTime));
  } else if (studyType == "BasicInfo") {
    sheet
      .getRange("C2")
      .setValue(Number(sheet.getRange("C2").getValue()) + Number(studyTime));
  } else if (studyType == "Programming") {
    sheet
      .getRange("D2")
      .setValue(Number(sheet.getRange("D2").getValue()) + Number(studyTime));
  } else if (studyType == "ReadingBooks") {
    sheet
      .getRange("E2")
      .setValue(Number(sheet.getRange("E2").getValue()) + Number(studyTime));
  } else if (studyType == "ContentConsumption") {
    sheet
      .getRange("F2")
      .setValue(Number(sheet.getRange("F2").getValue()) + Number(studyTime));
  }
}
