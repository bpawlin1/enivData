package com.environmentaldata.environmentaldata;

import net.sf.jasperreports.engine.DefaultJasperReportsContext;

import net.sf.jasperreports.engine.JasperCompileManager;

import net.sf.jasperreports.engine.JasperFillManager;
import net.sf.jasperreports.engine.JasperPrint;
import net.sf.jasperreports.engine.JasperReport;
import net.sf.jasperreports.engine.data.JRBeanCollectionDataSource;

import net.sf.jasperreports.engine.export.HtmlExporter;
import net.sf.jasperreports.export.SimpleExporterInput;
import net.sf.jasperreports.export.SimpleHtmlExporterOutput;

import java.io.IOException;
import java.io.InputStream;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;
import org.supercsv.io.CsvBeanWriter;
import org.supercsv.io.ICsvBeanWriter;
import org.supercsv.prefs.CsvPreference;


import com.google.gson.JsonArray;
import com.google.gson.JsonObject;




@Controller
public class App_Controller {
	final ModelAndView model = new ModelAndView();
	@Autowired
	co2_service service;
	co2Data_DAO dao;
	
	//Index and initial start page
	@RequestMapping("/")
	public String viewHomePage(Model model) {
		long count = service.count();
		model.addAttribute("count", count);
		System.out.println(count);
		
		List<co2_data> lastrecord = service.findFirstByOrderByDatetimeDesc();

		model.addAttribute("lastrecord", lastrecord);	
		

		model.addAttribute("lastrecord", lastrecord);		

		for (co2_data product : lastrecord) 
		{
			String date = product.getDatetime();
			DateFormat formatter = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
			DateFormat formatter1 = new SimpleDateFormat("MM/dd/yyyy hh:mm:ss aa");
			String dateFormatted ="";
			try {
				dateFormatted = (formatter1.format(formatter.parse(date)));
			} catch (ParseException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			String[] parts = dateFormatted.split(" ");
		    //System.out.println("Date: " + parts[0]);
		    model.addAttribute("date", parts[0]);
		    model.addAttribute("time", parts[1]);
		}
		
		List<co2_data> listdata = service.listAll();
		model.addAttribute("listdata", listdata);
		
		return "index";
	}
	
	@RequestMapping("/listResults")
	public String viewResults(Model model) {
		List<co2_data> listdata = service.listAll();
		model.addAttribute("listdata", listdata);
		
		return "ListResults";
	}
	
	//Co2 chart data
	@RequestMapping("/chart")
	@ResponseBody
	public String getChartData()
	{
		List<co2_data> listdata = service.findlast24Hrs();
		
		SimpleDateFormat format = new SimpleDateFormat("YYYY-MM-DD+HH:MM:SS");
		
		JsonArray jsonArrayCategory = new JsonArray();
		JsonArray jsonArraySeries = new JsonArray();
		JsonObject jsonObject = new JsonObject();
		
		listdata.forEach(data->
		{
			jsonArrayCategory.add(data.getCo2());
			jsonArraySeries.add(data.getDatetime());
		});
		
		jsonObject.add("co2", jsonArrayCategory);
		jsonObject.add("datetime", jsonArraySeries);
		
		return jsonObject.toString();
	}
	//temp chart data
	@RequestMapping("/tempchart")
	@ResponseBody
	public String getTempData()
	{
		List<co2_data> listdata = service.findFirstByOrderByDatetimeDesc();
	
		JsonArray jsonArraySeries = new JsonArray();
		JsonObject jsonObject = new JsonObject();
		
		listdata.forEach(data->
		{
			jsonArraySeries.add(data.getTemp());
			System.out.println(data.getTemp());
			
		});
	
		jsonObject.add("temp", jsonArraySeries);
		
		return jsonObject.toString();
	}

	
	@GetMapping("/admin")
    public String register()
    {
        return "admin";
    }
	
	
	//create PDF
    @RequestMapping(value = "/view", method = RequestMethod.GET)
	public void report(HttpServletResponse response) throws Exception {
		response.setContentType("text/html");
		JRBeanCollectionDataSource dataSource = new JRBeanCollectionDataSource(service.listAll());
		InputStream inputStream = this.getClass().getResourceAsStream("/enivadata2.jrxml");
		JasperReport jasperReport = JasperCompileManager.compileReport(inputStream);
		JasperPrint jasperPrint = JasperFillManager.fillReport(jasperReport, null, dataSource);
		HtmlExporter exporter = new HtmlExporter(DefaultJasperReportsContext.getInstance());
		exporter.setExporterInput(new SimpleExporterInput(jasperPrint));
		exporter.setExporterOutput(new SimpleHtmlExporterOutput(response.getWriter()));
		exporter.exportReport();
	}
    
    //Export to csv
    @GetMapping("/view/export")
    public void exportToCSV(HttpServletResponse response) throws IOException {
        response.setContentType("text/csv");
        DateFormat dateFormatter = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss");
        String currentDateTime = dateFormatter.format(new Date());
         
        String headerKey = "Content-Disposition";
        String headerValue = "attachment; filename=users_" + currentDateTime + ".csv";
        response.setHeader(headerKey, headerValue);
        
        List<co2_data> listData = service.listAll();
      
 
        ICsvBeanWriter csvWriter = new CsvBeanWriter(response.getWriter(), CsvPreference.STANDARD_PREFERENCE);
        String[] csvHeader = {"Record Id", "Device Name", "Date/Time", "Temperature","Humidity","Co2 Value","Co2 Rating"};
        String[] nameMapping = {"id", "device", "datetime", "temp", "humidity", "co2", "co2_rating"};
         
        csvWriter.writeHeader(csvHeader);
         
        for (co2_data record : listData) {
            csvWriter.write(record, nameMapping);
        }
         
        csvWriter.close();
}
}

