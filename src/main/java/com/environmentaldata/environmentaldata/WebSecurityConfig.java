package com.environmentaldata.environmentaldata;


	import javax.sql.DataSource;
	 
	import org.springframework.beans.factory.annotation.Autowired;
	import org.springframework.context.annotation.Configuration;
	import org.springframework.security.config.annotation.authentication.builders.*;
	import org.springframework.security.config.annotation.web.builders.HttpSecurity;
	import org.springframework.security.config.annotation.web.configuration.*;
	import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;

	 
	@Configuration
	@EnableWebSecurity
	public class WebSecurityConfig extends WebSecurityConfigurerAdapter {
	 
	    @Autowired
	    private DataSource dataSource;
	     
	    @SuppressWarnings("deprecation")
		@Autowired
	    public void configAuthentication(AuthenticationManagerBuilder auth) throws Exception {
	        auth.jdbcAuthentication().passwordEncoder(new BCryptPasswordEncoder())
	            .dataSource(dataSource)
	            .usersByUsernameQuery("select username, password, enabled from users where username=?")
	            .authoritiesByUsernameQuery("select username, role from users where username=?")
	        ;
	    }
	 
	    @Override
	    protected void configure(HttpSecurity http) throws Exception {
	        http.authorizeRequests()
	        	.antMatchers("/admin").hasRole("ADMIN")
	            .antMatchers("/", "/**", "index").access("permitAll")
	            .and()
	            .formLogin()
	            .and()
	            .logout();    
	    }
	}

