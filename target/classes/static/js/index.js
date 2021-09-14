$.ajax({

	url: "chart",
	
	success: function(result){
		
		var category = JSON.parse(result).co2;
		var series = JSON.parse(result).datetime;
		drawLineChart(category, series);
	
		
		}
})

/* for line chart */
function drawLineChart(category, series){
	
    Highcharts.chart('chart-area', {
      chart: {
        zoomType: 'x'
      },
      title: {
        text: 'CO2 Values Over Last 24 Hrs'
      },
    
    
      xAxis: {
		labels: {
        enabled: false
    }, 
        categories: series
      },

      yAxis: {
        title: {
          text: 'CO2 Value'
        }
      },
      legend: {
        enabled: false
      },
      plotOptions: {
        area: {
          fillColor: {
            linearGradient: {
              x1: 0,
              y1: 0,
              x2: 0,
              y2: 1
            },
            stops: [
              [0, Highcharts.getOptions().colors[0]],
              [1, Highcharts.color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
            ]
          },
          marker: {
            radius: 2
          },
          lineWidth: 1,
          states: {
            hover: {
              lineWidth: 1
            }
          },
          threshold: null
        }
      },

      series: [{
        type: 'area',
        name: 'CO2',
        data: category
      }]
    });
  }

