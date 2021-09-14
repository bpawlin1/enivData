package com.environmentaldata.environmentaldata;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

@Entity
public class users {

	private Long user_id;
	private String username;
	private String password;
	private String role;
	private int enabled;
	
	
	protected users() {
    }
	
	@Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
	public Long getUser_id() {
		return user_id;
	}
	public void setUser_id(Long user_id) {
		this.user_id = user_id;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getRole() {
		return role;
	}
	public void setRole(String role) {
		this.role = role;
	}
	public int getEnabled() {
		return enabled;
	}
	public void setEnabled(int enabled) {
		this.enabled = enabled;
	}
	
	@Override
	public String toString() {
		return "users [user_id=" + user_id + ", username=" + username + ", password=" + password + ", role=" + role
				+ ", enabled=" + enabled + ", UserType="  + "]";
	}
	
	
	
	
}
