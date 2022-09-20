package com.environmentaldata.environmentaldata;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;


@Entity
public class co2_data {

	
	private Long id;
	private String device;
	private String datetime;
	private double temp;
	private double humidity;
	private double co2;
	private String co2_rating;
	
	public String getCo2_rating() {
		return co2_rating;
	}

	public void setCo2_rating(String co2_rating) {
		
		this.co2_rating = co2_rating;
		
	}

	protected co2_data() {
    }
	
	@Override
	public String toString() {
		return "co2_table [Id=" + id + ", device=" + device + ", datetime=" + datetime + ", temp=" + temp
				+ ", humidity=" + humidity + ", co2=" + co2 + "]";
	}

	public void setId(Long id) {
		this.id = id;
	}

	@Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
	public Long getId() {
		return id;
	}
	
	
	
	public String getDevice() {
		return device;
	}



	public void setDevice(String device) {
		this.device = device;
	}



	public String getDatetime() {
		return datetime;
	}



	public void setDatetime(String datetime) {
		this.datetime = datetime;
	}



	public double getTemp() {
		return temp;
	}



	public void setTemp(double temp) {
		this.temp = temp;
	}



	public double getHumidity() {
		return humidity;
	}



	public void setHumidity(double humidity) {
		this.humidity = humidity;
	}



	public double getCo2() {
		return co2;
	}



	public void setCo2(double co2) {
		this.co2 = co2;
	}

	

	
}



