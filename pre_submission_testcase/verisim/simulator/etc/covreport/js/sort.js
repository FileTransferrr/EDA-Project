$(function() {
  const ths = $("th");

  ths.on("click", function() {
    if (this.myOrder === undefined) {
      this.myOrder = -1;
    }
    const table = $(this).parent().parent().parent();
    let tbody = table.children("tbody");
    let rows = $.makeArray(tbody.children("tr"));

    const col = this.cellIndex;
    rebuild(tbody, sortRows(rows, this.myOrder, col, $(this).text()));

    this.myOrder *= -1;
  });

  function sortRows(rows, order, col, title) {
    rows.sort(function(a, b) {
      return compare(a, b, col, title) * order;
    });
    return rows;
  }

  function toFloat(s) {
    if (s == "") return 0;
    let num = s.split(' ')[0];
    if (num[num.length-1] == "%") {
      num = num.substr(0, num.length-1);
    }
    return parseFloat(num);
  }

  function compare(a, b, col, title) {
    let _a = a.children[col].textContent;
    let _b = b.children[col].textContent;
    if (title != "Item" && title != "Name" && title != "Comment") {
      _a = toFloat(_a);
      _b = toFloat(_b);
    }
    return _a < _b ? -1 : (_a > _b ? 1 : 0);
  }

  function rebuild(tbody, rows) {
    while (tbody.firstChild) {
        tbody.remove(tbody.firstChild);
    }

    rows.forEach(function(row) {
        tbody.append(row);
    });
  }
})
